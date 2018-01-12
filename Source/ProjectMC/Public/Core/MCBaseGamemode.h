#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MCTeamManager.h"

#include "MCBaseGamemode.generated.h"

class AMCController;
class AMCCharacter;
class AMCPlayerState;

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	bool bIsReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	FString PlayerName;
};

UCLASS()
class PROJECTMC_API AMCBaseGamemode : public AGameMode
{
	GENERATED_BODY()

	TArray<FPlayerInfo> PlayerInfoArray;

	// Public Variables
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumTeams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ScorePerKill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString GameMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString GameMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AMCController*> ConnectedPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayTags)
	TArray<FString> GameplayTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> BannedClasses;

//	@removed(chrisr, 79)
// 	UPROPERTY(BlueprintReadWrite)
// 	TArray<FTeamData> TeamDataArray;

	UPROPERTY(BlueprintReadOnly)
	class UDataTable* TeamDataTable;

	// Public Functions
public:

	AMCBaseGamemode();

	// @removed(chrisr, 79): this function has been removed and refactored. Keeping the code because it may be used later, refer to KilledBy(APawn* Killer) in MCCharacter.h
	// void KilledBy(AController* Killer, AActor* DamageCauser, AController* KilledController, AActor* KilledActor);

	void NotifyKills(AController* Killer, AController* KilledPlayer, APawn* KilledPawn, const struct FGameplayEffectSpec& KillingEffect);

	void BroadcastMessage(const FString& ChatMessage);

	void PlayerRequestUpdate();

	void UpdatePlayerList();

	void StartGameFromLobby();

	bool GetAllPlayersReady() const;

	bool IsTeamFull();

protected:

	// use protection /lennyface
protected:

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;

	virtual APlayerController* ProcessClientTravel(FString& URL, FGuid NextMapGuid, bool bSeamless, bool bAbsolute) override;

	virtual void BeginPlay() override;

	virtual void PreInitializeComponents() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void InitGameState() override;

	virtual int32 GetNumPlayers() override;

	virtual void StartPlay() override;

	virtual void ResetLevel() override;

	virtual bool CanServerTravel(const FString& URL, bool bAbsolute) override;

	virtual void ProcessServerTravel(const FString& URL, bool bAbsolute = false) override;

	virtual void GameWelcomePlayer(UNetConnection* Connection, FString& RedirectURL) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;

	virtual void SetPlayerDefaults(APawn* PlayerPawn) override;

	virtual void Reset() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostLoad() override;

	AMCPlayerState* GetPlayerState(APlayerState* NewPS);
};
