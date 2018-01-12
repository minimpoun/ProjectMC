#pragma once

#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"

#include "MCGameplayAbilitySet.generated.h"

UENUM(BlueprintType)
namespace EMCInputBinds
{
	enum MCBinds
	{
		BasicAttack,
		SpecialAttack,
		Ability1,
		Ability2,
		Ability3,
		Ability4
	};
}

USTRUCT(BlueprintType)
struct FMCBindInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMCInputBinds::MCBinds> InputBinds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> GameplayAbility;
};

UCLASS()
class PROJECTMC_API UMCGameplayAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Set")
	TArray<FMCBindInfo> ClassAbilities;

	void GiveAbilities(class UAbilitySystemComponent* AbilitySystemComponent) const;
};