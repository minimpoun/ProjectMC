#include "MCCharacter.h"
#include "MCController.h"
#include "MCPlayerState.h"
#include "MCCoreAttributeSet.h"
#include "MCGameplayAbilitySet.h"
#include "MCMainTypes.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilitiesModule.h"

#include "Net/UnrealNetwork.h"
#include "UnrealType.h"
#include "Engine/DataTable.h"
#include "EngineUtils.h"
#include "Engine/EngineTypes.h"
#include "ConstructorHelpers.h"

AMCCharacter::AMCCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicateMovement = true;
	bReplicates = true;
	bIsDying = false;

	CameraZoomModifier = 25.f;
	MinZoomDistance = 70.f;
	MaxZoomDistance = 650.f;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.f;
	
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 300.f;
	CameraArm->bUsePawnControlRotation = true; 

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);
	Camera->bUsePawnControlRotation = false;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UMCCoreAttributeSet>(TEXT("Attribute Set"));
}

void AMCCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("SwitchToNextTarget", IE_Pressed, this, &AMCCharacter::SwitchToNextTarget);
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AMCCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AMCCharacter::ZoomOut);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMCCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbiliyInputBinds("ConfirmInput", "CancelInput", "EMCInputBinds"));
}

uint8 AMCCharacter::GetTeamID() const
{
	AMCPlayerState* MCPS = GetPlayerState();
	return MCPS ? MCPS->TeamID : TEAM_None;
}

AMCPlayerState* AMCCharacter::GetPlayerState() const
{
	return Cast<AMCPlayerState>(PlayerState);
}

bool AMCCharacter::CanDie() const
{
	if (bIsDying
		|| Role != ROLE_Authority
		|| !MCBaseGamemode
		|| MCBaseGamemode->GetMatchState() == MatchState::LeavingMap
		|| MCBaseGamemode->GetMatchState() == MatchState::WaitingPostMatch
		|| IsPendingKill()
		)
	{
		return false;
	}

	return true;
}

bool AMCCharacter::HandleDeferredDeathLogic(float KillingDamageMagnitude, const FGameplayEffectSpec& KillingEffect, AController* Killer, AActor* DamageCauser)
{
	if (!CanDie())
	{
		return false;
	}
	 
	NetUpdateFrequency = MCBaseGamemode->NetUpdateFrequency;
	GetCharacterMovement()->ForceReplicationUpdate();

	OnDeath(KillingDamageMagnitude, KillingEffect, Killer, DamageCauser);

	return true;
}

void AMCCharacter::OnDeath(float KillingDamageMagnitude, const FGameplayEffectSpec& KillingEffect, AController* Killer, AActor* DamageCauser)
{
	if (bIsDying)
	{
		return;
	}
	
	bReplicateMovement = false;
	bTearOff = true;
	bIsDying = true;

	if (Role == ROLE_Authority)
	{
		// @todo(chrisr): add impulse 
	}

	GetCharacterMovement()->DisableMovement();

	USkeletalMeshComponent* L_Mesh = GetMesh();
	if (GetNetMode() != NM_DedicatedServer
		&& DeathSound
		&& L_Mesh
		&& L_Mesh->IsVisible()
	   )
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	DetachFromControllerPendingDestroy();
	StopAllMontages();

	if (L_Mesh)
	{
		static FName CollisionType(TEXT("Ragdoll"));
		L_Mesh->SetCollisionProfileName(CollisionType);
	}

	SetActorEnableCollision(true);

	float DeathAnimMontage_Duration = PlayAnimMontage(DeathAnimMontage);
	if (DeathAnimMontage_Duration > 0)
	{
		const float RagdollDelay = DeathAnimMontage_Duration - 0.7f;
		L_Mesh->bBlendPhysics = true;
		
		GetWorldTimerManager().SetTimer(TimerHandle_Dying, this, &AMCCharacter::Ragdoll, FMath::Max(0.1f, RagdollDelay), false);
	}
	else
	{
		Ragdoll();
	}

	GetCharacterMovement()->StopMovementImmediately();

	if (LowHealthSoundHook && LowHealthSoundHook->IsPlaying())
	{
		LowHealthSoundHook->Stop();
	}

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	AController* const KilledController = (Controller != NULL) ? Controller : Cast<AController>(GetOwner());
	MCBaseGamemode->NotifyKills(Killer, KilledController, this, KillingEffect);
}

void AMCCharacter::KilledBy(APawn* Killer)
{
	if (!bIsDying && Role == ROLE_Authority && Killer)
	{
		AController* Controller = nullptr;
		Controller = Killer->GetController();
	}
}

float AMCCharacter::GetHealthIfLowPercentage() const
{
	return LowHealthStartLevel;
}

void AMCCharacter::Ragdoll()
{
	bool bRagdoll = false;

	if (IsPendingKill())
	{
		bRagdoll = false;
	}
	else if (!GetMesh() || !GetMesh()->GetPhysicsAsset())
	{
		bRagdoll = false;
	}
	else
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->bBlendPhysics = true;
		GetMesh()->WakeAllRigidBodies();
		bRagdoll = true;
	}

	if (!bRagdoll)
	{
		TornOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(150.f);
	}
	else
	{
		SetLifeSpan(180.f);
	}
}

float AMCCharacter::GetCurrentHealth() const
{
	return AttributeSet->Health.GetCurrentValue();
}

float AMCCharacter::GetMaxHealth() const
{
	return AttributeSet->Health.GetBaseValue();
}

void AMCCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	const FGameplayEffectSpec NULLEffect;
	HandleDeferredDeathLogic(AttributeSet->Health.GetBaseValue(), NULLEffect, NULL, NULL);
}

void AMCCharacter::MoveRight(float Val)
{
	if (Controller && Val != 0.f)
	{
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator YawRot(0, Rot.Yaw, 0);
		const FVector Dir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		AddMovementInput(Dir, Val);
	}
}

void AMCCharacter::MoveForward(float Val)
{
	if (Controller && Val != 0.f)
	{
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator YawRot(0, Rot.Yaw, 0);
		const FVector Dir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		AddMovementInput(Dir, Val);
	}
}

void AMCCharacter::SwitchToNextTarget()
{
	CurrentTarget = SearchNextTarget(ETabTargetSearchType::Search_ScreenDistance_Crosshair);
}

void AMCCharacter::OnRep_CurrentTarget()
{

}

AMCCharacter* AMCCharacter::SearchNextTarget(ETabTargetSearchType SearchType)
{	
	float ClosestMetric = MAX_FLT;
	AMCCharacter* ClosestActor = nullptr;

	for (TActorIterator<AMCCharacter> It(GetWorld()); It; ++It)
	{
		AMCCharacter* OtherChar = *It;

		if (GetTeamID() == OtherChar->GetTeamID()
			|| OtherChar->IsDead()
			|| !OtherChar->WasRecentlyRendered(0.1f)
			|| ((OtherChar->GetActorLocation() - GetActorLocation()) | GetActorForwardVector()) < 0.f) // @note(devlinw): should this instead be control rot or otherwise view based? 
		{
			continue;
		}

		float TempMetric = MAX_FLT;
		bool bCheckYAxis = false;

		switch (SearchType)
		{
			default:
			{
				return ClosestActor; //	@todo(devlinw):	#logging
			}
			case ETabTargetSearchType::Search_WorldDistance:
			{
				TempMetric = GetDistanceTo(OtherChar);
			} break;
			case ETabTargetSearchType::Search_ScreenDistance_Crosshair:
				bCheckYAxis = true; // the rest of the logic is the same, so fall through.
			case ETabTargetSearchType::Search_ScreenDistance_ScreenCenter:
			{
				TempMetric = GetDistanceToScreenCenter(OtherChar, bCheckYAxis);

				if (TempMetric == -1.f)
				{
					return ClosestActor; //	@todo(devlinw): #loggging
				}
			} break;
		}

		if (TempMetric < ClosestMetric)
		{
			ClosestMetric = TempMetric;
			ClosestActor = OtherChar;
		}
	}

	return ClosestActor;
}

float AMCCharacter::GetDistanceToScreenCenter(AMCCharacter* OtherChar, bool bCheckYAxis /*= true*/)
{
	float Distance = -1.f;

	// #SplitScreen Fix up the project and viewport center accessors
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FVector2D ScreenLocation;
		PC->ProjectWorldLocationToScreen(OtherChar->GetActorLocation(), ScreenLocation);

		FVector2D ViewportCenter;

		ULocalPlayer* LocPlayer = Cast<ULocalPlayer>(PC->Player);
		if (LocPlayer && LocPlayer->ViewportClient)
		{
			LocPlayer->ViewportClient->GetViewportSize(ViewportCenter);

			ViewportCenter /= 2.f;

			if (!bCheckYAxis)
			{
				ScreenLocation.Y = ViewportCenter.Y;
			}

			Distance = (ScreenLocation - ViewportCenter).Size();
		}
	}
	return Distance;
}

void AMCCharacter::StopAllMontages()
{
	USkeletalMeshComponent* L_Mesh = GetMesh();
	if (L_Mesh && L_Mesh->AnimScriptInstance)
	{
		L_Mesh->AnimScriptInstance->Montage_Stop(0.f);
	}
}

void AMCCharacter::ZoomIn()
{
	CameraZoom = CameraZoom - CameraZoomModifier;
	if (CameraZoom <= MinZoomDistance)
	{
		CameraZoom = MinZoomDistance;
		CameraArm->TargetArmLength = MinZoomDistance;
	}
	else
	{
		CameraArm->TargetArmLength = CameraZoom;
	}
}

void AMCCharacter::ZoomOut()
{
	CameraZoom = CameraZoom + CameraZoomModifier;
	if (CameraZoom >= MaxZoomDistance)
	{
		CameraZoom = MaxZoomDistance;
		CameraArm->TargetArmLength = MaxZoomDistance;
	}
	else
	{
		CameraArm->TargetArmLength = CameraZoom;
	}
}

void AMCCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (GetPlayerState())
	{
		
	}
}

void AMCCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AMCCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
}

void AMCCharacter::TornOff()
{
	Super::TornOff();

	if (!bIsDying && !IsPendingKill())
	{
		
	}
}

void AMCCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AMCBaseGamemode* GM = Cast<AMCBaseGamemode>(GetWorld()->GetAuthGameMode()))
	{
		MCBaseGamemode = GM;
	}

	if (AMCController* PC = Cast<AMCController>(GetController()))
	{
		MCController = PC;
	}
}

void AMCCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (AbilitySystemComponent)
	{
		if (Role == ROLE_Authority && AbilitySet != nullptr)
		{
			AbilitySet->GiveAbilities(AbilitySystemComponent);
		}
	}
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AMCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMCCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMCCharacter, CurrentTarget);
}
