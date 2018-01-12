#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MCAttributeMacros.h"

#include "MCCoreAttributeSet.generated.h"

UCLASS(BlueprintType)
class PROJECTMC_API UMCCoreAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	/// Default Stats everyone has
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData Mana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData DefenseRating;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData MovementSpeed;

	/// None Replicated Stats

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Core Stats")
	FGameplayAttributeData HealthRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Core Stats")
	FGameplayAttributeData ManaRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Core Stats")
	FGameplayAttributeData GCD;

	/// Types of Damage

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData Fire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData Water; // wetness ( ͡° ͜ʖ ͡°)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData Ice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData Blunt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData Bleed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData Energy;

	/// Stats that can be buffed from things like armor/gear

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData Endurance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData MagicResistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData CritChance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData CritBonus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Core Stats")
	FGameplayAttributeData PowerBonus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Category = "Core Stats")
	FGameplayAttributeData TransientEffectPower;

	// Register the stats
	DECLARE_ATTRIBUTE_FUNCTION(Health);
	DECLARE_ATTRIBUTE_FUNCTION(Mana);
	DECLARE_ATTRIBUTE_FUNCTION(DefenseRating);
	DECLARE_ATTRIBUTE_FUNCTION(MovementSpeed);
	DECLARE_ATTRIBUTE_FUNCTION(HealthRegenRate);
	DECLARE_ATTRIBUTE_FUNCTION(ManaRegenRate);
	DECLARE_ATTRIBUTE_FUNCTION(GCD);
	DECLARE_ATTRIBUTE_FUNCTION(Fire);
	DECLARE_ATTRIBUTE_FUNCTION(Water);
	DECLARE_ATTRIBUTE_FUNCTION(Ice);
	DECLARE_ATTRIBUTE_FUNCTION(Blunt);
	DECLARE_ATTRIBUTE_FUNCTION(Bleed);
	DECLARE_ATTRIBUTE_FUNCTION(Energy);
	DECLARE_ATTRIBUTE_FUNCTION(Endurance);
	DECLARE_ATTRIBUTE_FUNCTION(MagicResistance);
	DECLARE_ATTRIBUTE_FUNCTION(CritChance);
	DECLARE_ATTRIBUTE_FUNCTION(CritBonus);
	DECLARE_ATTRIBUTE_FUNCTION(AttackPower);
	DECLARE_ATTRIBUTE_FUNCTION(PowerBonus);
	DECLARE_ATTRIBUTE_FUNCTION(TransientEffectPower);

protected:

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};