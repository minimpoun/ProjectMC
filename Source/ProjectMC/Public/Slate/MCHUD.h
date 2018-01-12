#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "MCHUD.generated.h"

struct FColor;

// @note(chrisr, devlinw): Hey Devlin, what do you think of the way I did this? I feel like it's kind of weird, but at the same time shit compiles yo. /shrug
USTRUCT(BlueprintType)
struct FMessageContainer
{
	GENERATED_BODY()

	FMessageContainer()
		: KillerTeamID(0)
		, VictimTeamID(0)
		, bIsKillerLocal(false)
		{}

	UPROPERTY(BlueprintReadWrite)
	FString KillerName;

	UPROPERTY(BlueprintReadWrite)
	FString VictimName;

	UPROPERTY(BlueprintReadWrite)
	uint8 KillerTeamID;

	UPROPERTY(BlueprintReadWrite)
	uint8 VictimTeamID;

	UPROPERTY(BlueprintReadWrite)
	bool bIsKillerLocal;
};

UCLASS()
class PROJECTMC_API AMCHUD : public AHUD
{
	GENERATED_BODY()

public:

	void DisplayKillToast(class AMCPlayerState* KillerPS, class AMCPlayerState* VictimPS);



protected:

	UPROPERTY(BlueprintReadOnly)
	TArray<FMessageContainer> ActiveMessages;

	FText KilledPlayerMessage;

	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	int32 MaxVisibleMessages;

	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	float MessageDurationInSeconds;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCreateKillFeedMessage(const FMessageContainer& Message, const float& HideTimer);

	virtual void BeginPlay() override;

	void DrawDeathMessage();

	UFUNCTION(BlueprintPure, Category = "Getters for TeamInfo")
	FColor GetTeamColorFromID(const uint8 TeamID);

	UFUNCTION(BlueprintPure, Category = "Getters for TeamInfo")
	FString GetTeamNameFromID(const uint8 TeamID);
};