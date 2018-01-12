#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MCBaseGamemode.h"
#include "MCDependancies_GS.h"
#include "MCMainTypes.h"

#include "MCController.generated.h"

class AMCCharacter;
class AMCHUD;

UCLASS(config=Game)
class PROJECTMC_API AMCController : public APlayerController
{
	GENERATED_BODY()

	// Public Variables
public:

	UPROPERTY(BlueprintReadWrite, Category = "Class Settings")
	FCharacterData CharacterData;

	UFUNCTION()
	void OnGameSparksAvailable(bool available);
	
	static void AuthenticationRequest_Response(GS_Global& GS_Global, const GS_AuthResponse& GS_AuthResponse);

protected:

	/************************************************************************/
	/* Slate Shit                                                           */
	/************************************************************************/

	FORCEINLINE AMCHUD* GetMCHUD() const;

	// Public Functions
public:

	AMCController(const FObjectInitializer& ObjectInitializer);

	// @note(chrisr): Only thing we are actually doing here right is showing a kill message to the screen.
	// I went ahead and added this function instead of just doing it all in the PS/GM because we may want to do more here in the future.
	void OnBroadcastDeathMessage(class AMCPlayerState* KillerPS, class AMCPlayerState* VictimPS);

	// Returns true if the Game is able to start
	// All players ready ex.
	UFUNCTION(BlueprintPure, Category = "Wrapper Functions")
	bool CanGameStart() const;

	// This is called to actually start the game
	UFUNCTION(BlueprintCallable, Category = "Wrapper Functions")
	void StartGame();

	// Sends a FString& to all connected players.
	// @todo(chrisr): Team based chat
	UFUNCTION(BlueprintCallable, Category = "Wrapper Functions")
	void SendMessage(const FString& ChatMessage);

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerSendMessage(const FString& ChatMessage);
	void ServerSendMessage_Implementation(const FString& ChatMessage);
	FORCEINLINE bool ServerSendMessage_Validate(const FString& ChatMessage) 
	{
		return true;
	}

	// This function gets called whenever the client gets a new message
	UFUNCTION(BlueprintImplementableEvent, Category = "Wrapper Functions")
	void ReceiveChatMessage(const FString& ChatMessage);

	UFUNCTION(Client, Unreliable)
	void ClientReceviedMessage(const FString& ChatMessage);
	void ClientReceviedMessage_Implementation(const FString& ChatMessage);
	FORCEINLINE bool ClientReceviedMessage_Validate(const FString& ChatMessage) 
	{ 
		return true; 
	}

	// This gets called whenever the player hits refresh on the player screen OR someone leaves/joins the game
	UFUNCTION(BlueprintImplementableEvent, Category = "Wrapper Functions")
	void UpdatePlayerList(const TArray<FPlayerInfo>& ConnectedPlayerInfo);

	UFUNCTION(Client, Unreliable)
	void ClientUpdatePlayerList(const TArray<FPlayerInfo>& ConnectedPlayerInfo);
	void ClientUpdatePlayerList_Implementation(const TArray<FPlayerInfo>& ConnectedPlayerInfo);

	// Called whenever the PlayerList should update
	void RequestPlayerListUpdate();

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerRequestPlayerListUpdate();
	void ServerRequestPlayerListUpdate_Implementation();
	FORCEINLINE bool ServerRequestPlayerListUpdate_Validate()
	{ 
		return true; 
	}

	// Sets ready state
	// holy fucking shit mind blown
	UFUNCTION(BlueprintCallable, Category = "Wrapper Functions")
	void SetReadyState(bool bNewReadyState);

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerSetReadyState(bool bNewReadyState);
	void ServerSetReadyState_Implementation(bool bNewReadyState);
	FORCEINLINE bool ServerSetReadyState_Validate(bool bNewReadyState) 
	{ 
		return true; 
	}

	// Variables
protected:

	UPROPERTY(EditDefaultsOnly)
	class AMCHUD* HUDClass;

	// functions
protected:

	void ToggleHUD();

	void OnToggleScoreboard();

	void OnHideScoreboard();

	virtual void BeginPlay() override;

	virtual void Tick( float DeltaSeconds ) override;

	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintReadOnly)
	AMCCharacter* CharacterRef;

	// removed shit
public:
	// @removed(chrisr, 79): removed in-game achievement support due to that fact we'll most likely use steam/GameSparks. 
	// Also, no point in going through the trouble of doing this right now. It's something we can do later. 
	// 	/*
	// 	 *	Achievements
	// 	 **/
	// 	void UpdateAchievementProgress(const FString& AchievementID, float PrecentToUpdate);
	// 	void QueryAchievementList();
	// 	void OnQueryAchievementsComplete(const FUniqueNetId& UserID, const bool bWasSUCCessful); // my work here is done
	// 	/*
	// 	 *	End Achievements
	// 	 **/
	// 	 FOnlineAchievementsWritePtr WriteObjectPtr;
	// 	 FVector LastDeathLoc;
private:
protected:
};