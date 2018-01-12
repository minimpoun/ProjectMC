#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "MCCharacter.generated.h"

class AMCController;
class AMCPlayerState;
class AMCBaseGamemode;

class UMCCoreAttributeSet;
class UMCGameplayAbilitySet;
class UAttributeSet;
class UAbilitySystemComponent;

class USpringArmComponent;
class UCameraComponent;
class USkeletalMesh;
class UArrowComponent;

class UParticleSystem;
class UUserWidget;
class UDataAsset;
class UTexture2D;
class USoundBase;
class UAnimMontage;

UENUM(BlueprintType)
enum class ETabTargetSearchType : uint8
{
	Search_WorldDistance,
	Search_ScreenDistance_Crosshair,
	Search_ScreenDistance_ScreenCenter,
};

UCLASS(Transient, Abstract, notplaceable, config = Game)
class PROJECTMC_API AMCCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	UParticleSystem* CharacterTrail;

public:

	AMCCharacter(const FObjectInitializer& ObjectInitializer);

	/************************************************************************/
	/*						 Health, Death and Damage                       */
	/************************************************************************/

	UPROPERTY(BlueprintReadWrite, Category = "Player Defaults")
	USoundBase* DeathSound;

	UPROPERTY(BlueprintReadWrite, Category = "Player Defaults")
	USoundBase* LowHealthSound;

	UPROPERTY(BlueprintReadWrite, Category = "Player Defaults")
	USoundBase* LowHealthWarningSound;

	UPROPERTY(BlueprintReadWrite, Category = "Player Defaults")
	UAudioComponent* LowHealthSoundHook;

	UPROPERTY(BlueprintReadWrite, Category = "Player Defaults")
	UAnimMontage* DeathAnimMontage;

	bool bIsDying;

	FTimerHandle TimerHandle_Dying;

	float LowHealthStartLevel;
	
	///////////////////////////////^^ Variables ^^//////////////////////////
	///////////////////////////////vv Functions vv//////////////////////////
	
	/*
	 *	Returns true if the logic has finished. Will return false if the character cannot die for some reason. Do not mistake this for CanDie()
	 *	This function does not check to see if the character can die, it handles death logic we don't want OnDeath() for one reason or another.
	 **/
	virtual bool HandleDeferredDeathLogic(float KillingDamageMagnitude, const struct FGameplayEffectSpec& KillingEffect, AController* Killer, AActor* DamageCauser);

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	virtual void OnDeath(float KillingDamageMagnitude, const FGameplayEffectSpec& KillingEffect, AController* Killer, AActor* DamageCauser);

	// this takes a Pawn so we can have other things kill the player other than our Character class. Ex: Turrets, or AI
	virtual void KilledBy(APawn* Killer);

	// @note(chrisr): CanDie doesn't use any of these params as of right now, but I added them because we may want to do checks based off of incoming damage or the type of damage effect being applied
	bool CanDie() const;

	FORCEINLINE virtual bool IsDead() const
	{
		return bIsDying;
	}

	float GetHealthIfLowPercentage() const;

	void Ragdoll();

	// These are not inlined so we don't have to include MCCoreAttributeSet
	float GetCurrentHealth() const;
	float GetMaxHealth() const;

	/************************************************************************/
	/*						 End Health, Death and Damage                   */
	/************************************************************************/

	FORCEINLINE virtual uint8 GetTeamID() const;
	FORCEINLINE virtual AMCPlayerState* GetPlayerState() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	UAbilitySystemComponent* AbilitySystemComponent;

	FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

	// Name of this class, ex: Warrior/Thief
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ClassName;

	UPROPERTY()
	UMCCoreAttributeSet* AttributeSet;

	// The set of Abilities this class has access too
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMCGameplayAbilitySet* AbilitySet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAttributeSet> ClassStats;

	/* Camera Zoom Variables */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CameraZoomModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinZoomDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxZoomDistance;

	float CameraZoom;
	
protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Targeting, ReplicatedUsing=OnRep_CurrentTarget)
	AMCCharacter* CurrentTarget;

	UPROPERTY(BlueprintReadOnly, Category = ClassPointers)
	AMCController* MCController;

	UPROPERTY(BlueprintReadOnly, Category = ClassPointers)
	AMCBaseGamemode* MCBaseGamemode;

	void StopAllMontages();

	void ZoomIn();
	void ZoomOut();
	void MoveRight(float Val);
	void MoveForward(float Val);

	void SwitchToNextTarget();

	UFUNCTION()
	void OnRep_CurrentTarget();

	AMCCharacter* SearchNextTarget(ETabTargetSearchType SearchType);

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAbilitySystemComponent* GetTargetAbilitySystemComponent() const
	{
		return CurrentTarget->AbilitySystemComponent;
	}

	float GetDistanceToScreenCenter(AMCCharacter* OtherChar, bool bCheckYAxis = true);
	
	virtual void OnRep_PlayerState() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_Controller() override;

	virtual void TornOff() override;

	virtual void BeginPlay() override;

	virtual void PreInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};