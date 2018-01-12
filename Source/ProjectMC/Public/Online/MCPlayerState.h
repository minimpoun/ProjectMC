#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "MCPlayerState.generated.h"

#define TEAM_Invalid 0
#define TEAM_None 255

UCLASS()
class PROJECTMC_API AMCPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	void SetTeamColor();

	void AddKill(AMCPlayerState* Victim, int32 NewKill);
	void AddDeath(AMCPlayerState* Killer, int32 NewDeath);
	void AddScore(float NewScore);

	void SetTeam(uint8 NewTeam);

	FORCEINLINE uint8 GetTeam() const
	{
		return TeamID;
	}

	FORCEINLINE int32 GetKills() const
	{
		return CurrentKills;
	}

	FORCEINLINE int32 GetDeath() const
	{
		return CurrentDeaths;
	}

	FORCEINLINE float GetTotalScore() const
	{
		return TotalScore;
	}

	void UpdateQuitterStatus(bool Val);

	FORCEINLINE bool GetIsQuitter() const
	{
		return bIsMatchQuitter;
	}

	UFUNCTION(Unreliable, Client)
	void AnnounceKill(class AMCPlayerState* Killer, class AMCPlayerState* Victim);
	void AnnounceKill_Implementation(class AMCPlayerState* Killer, class AMCPlayerState* Victim);

	// @todo(chrisr): Look into doing this in a different way. Cause, muh multicasts
	UFUNCTION(Unreliable, NetMulticast)
	void AnnounceDeath(class AMCPlayerState* Killer, class AMCPlayerState* Victim);
	void AnnounceDeath_Implementation(class AMCPlayerState* Killer, class AMCPlayerState* Victim);

	UFUNCTION()
	FString GetShortendPlayerName() const;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bIsReady;

	UPROPERTY()
	bool bIsMatchQuitter;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 TeamID;

	UPROPERTY(BlueprintReadOnly, Replicated, Transient, Category = "Player Stats")
	int32 CurrentKills;

	UPROPERTY(BlueprintReadOnly, Replicated, Transient, Category = "Player Stats")
	int32 CurrentDeaths;

	UPROPERTY(BlueprintReadOnly, Replicated, Transient, Category = "Player Stats")
	float TotalScore;

	UPROPERTY(BlueprintReadOnly, Replicated, Transient, Category = "Player Stats")
	float TotalDamage;
		
	UPROPERTY(BlueprintReadOnly, Replicated, Transient, Category = "Player Stats")
	float TotalHealing;

	UPROPERTY(BlueprintReadOnly, Replicated, Transient, Category = "Player Stats")
	float TotalProtection;

	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	int32 MaxNameLength;

protected:

	virtual void UnregisterPlayerWithSession() override;

	virtual void ClientInitialize(class AController* C) override;

	virtual void CopyProperties(APlayerState* PlayerState) override;

	virtual void Reset() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};