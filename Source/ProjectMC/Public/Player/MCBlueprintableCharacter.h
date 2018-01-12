#pragma once

#include "CoreMinimal.h"
#include "MCCharacter.h"

#include "MCBlueprintableCharacter.generated.h"

UCLASS(nonTransient, Blueprintable, BlueprintType, placeable)
class PROJECTMC_API AMCBlueprintableCharacter : public AMCCharacter
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};