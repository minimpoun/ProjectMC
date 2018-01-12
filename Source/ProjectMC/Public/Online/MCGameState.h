#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "MCGameState.generated.h"

UCLASS()
class PROJECTMC_API AMCGameState : public AGameState
{
	GENERATED_BODY()

public:

	AMCGameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	TArray<FName> LockedCharacters;

	UPROPERTY(BlueprintReadOnly, Category = TeamInfo)
	TArray<FColor> TeamColors;

	UPROPERTY(BlueprintReadOnly, Category = TeamInfo)
	TArray<FString> TeamNames;

	void SetupTeams(int32 NumOfTeams);
};