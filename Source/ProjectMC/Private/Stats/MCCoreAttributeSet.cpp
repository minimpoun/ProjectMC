#include "MCCoreAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "MCCharacter.h"

DEFINE_ATTRIBUTE_FUNCTION(Health, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Mana, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(DefenseRating, UMCCoreAttributeSet); 
DEFINE_ATTRIBUTE_FUNCTION(MovementSpeed, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(HealthRegenRate, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(ManaRegenRate, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(GCD, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Fire, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Water, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Ice, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Blunt, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Bleed, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Energy, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Endurance, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(MagicResistance, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(CritChance, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(CritBonus, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(AttackPower, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(PowerBonus, UMCCoreAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(TransientEffectPower, UMCCoreAttributeSet);

bool UMCCoreAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data)
{
	Super::PreGameplayEffectExecute(Data);

	return true;
}

void UMCCoreAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	Super::PostGameplayEffectExecute(Data);

	UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	AActor* AttackedActor = nullptr;
	AController* AttackedController = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AttackedActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		AttackedController = Data.Target.AbilityActorInfo->PlayerController.Get();
	}

	AActor* AttackingActor = nullptr;
	AController* AttackingController = nullptr;

	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		AttackingActor = Source->AbilityActorInfo->AvatarActor.Get();
		AttackingController = Source->AbilityActorInfo->PlayerController.Get();
		
		if (AttackingController == nullptr && AttackingActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(AttackingActor))
			{
				AttackingController = Pawn->GetController();
			}
		}
	}

	if (HealthAttribute() == Data.EvaluatedData.Attribute)
	{
		Health = FMath::Clamp(Health.GetCurrentValue(), 0.f, Health.GetBaseValue());
		if (Health.GetCurrentValue() <= 0.f)
		{
			if (AMCCharacter* Character = Cast<AMCCharacter>(this->GetOwningActor()))
			{
				if (Character->CanDie())
				{
					Character->HandleDeferredDeathLogic(0.f /* @note(chrisr): just passing in zero right now, we can support magnitude later*/, Data.EffectSpec, AttackingController, AttackingActor);
				}
			}
		}
	}
}

void UMCCoreAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UMCCoreAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UMCCoreAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, DefenseRating, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, Fire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, Water, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, Ice, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, Blunt, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, Bleed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, CritBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCCoreAttributeSet, PowerBonus, COND_None, REPNOTIFY_Always);
}