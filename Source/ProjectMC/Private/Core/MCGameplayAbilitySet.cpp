#include "MCGameplayAbilitySet.h"
#include "AbilitySystemComponent.h"

void UMCGameplayAbilitySet::GiveAbilities(class UAbilitySystemComponent* AbilitySystemComponent) const
{
	for (const FMCBindInfo& BindInfo : ClassAbilities)
	{
		if (BindInfo.GameplayAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BindInfo.GameplayAbility->GetDefaultObject<UGameplayAbility>(), 1, (int32)BindInfo.InputBinds));
		}
	}
}