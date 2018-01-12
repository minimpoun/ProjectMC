#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Online.h"
#include "MCDependancies_GS.h"

#include "MCGameInstance.generated.h"

#define SETTING_SERVER_NAME FName(TEXT("SERVERNAMEKEY"))

// @TODO(chrisr): Finish documentation on this class 

USTRUCT(BlueprintType)
struct FSearchResultBPWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SessionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLAN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentConnectedPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxNumberOfPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPasswordProtected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SessionPassword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Ping;
};

class UGameSparksObject;
class UGSMessageListenersObject;

UCLASS(config=Game)
class PROJECTMC_API UMCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	/*
	 *
	 *	if you need documentation on this, just quit game dev
	 *
	 **/
	UMCGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gamesparks")
	UGameSparksObject* GamesparksObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gamesparks")
	UGSMessageListenersObject* GamesparksMessageListener;

	virtual void Init() override;

protected:

	/*
	 *	Searching for Sessions 
	 **/

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearchSettings;

	/*
	 * Main function used to setup search settings, calls OnlineSessionInterfaceSteam::FindSession
	 * 
	 * @param UserID:
	 * @param bIsPresence:
	 * @param MaxSearchResults:
	 * @param bIsDedicated:
	 * @param TimeoutInSeconds:
	 * 
	 **/
	void FindSession(TSharedPtr<const FUniqueNetId> UserID, bool bIsPresence, bool bIsLAN, int32 MaxSearchResults, bool bIsDedicated, float TimeoutInSeconds);

	/*
	 * This function handles getting session search results. We use this information to create a server browser if that's going to be a thing
	 * 
	 * @param bWasSuccessful: Returns true if at least one valid session was found.
	 *
	 **/
	void OnFindSessionsComplete(bool bWasSuccessful);
	
	/*
	 *
	 *	@param SearchResults:
	 *
	 **/
	UFUNCTION(BlueprintImplementableEvent, Category = "Session Wrappers", meta = (DisplayName = "Display Session Search Results"))
	void K2_OnFoundSessionsComplete(const TArray<FSearchResultBPWrapper>& SearchResults);

	/*
	 *	
	 *	@param bIsPresence:
	 *	@param MaxSearchResults:
	 *	@param bIsDedicated:
	 *	@param TimeoutInSeconds:
	 *	
	 **/
	UFUNCTION(BlueprintCallable, Category = "Session Wrappers", meta = (DisplayName = "Find Sessions Wrapper"))
	void K2_FindSession(bool bIsPresence, bool bIsLAN, int32 MaxSearchResults, bool bIsDedicated, float TimeoutInSeconds);

	/*
	 *	Joining Sessions
	 **/

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/*
	 *	Main JoinSession function.
	 *	@note(chrisr): @jackie this is the function that gets called when a user finds a match and wants to join
	 *	
	 *	@param UserID:
	 *	@param SessionName:
	 *	@param SearchResult:
	 *	
	 **/
	bool JoinFoundSession(TSharedPtr<const FUniqueNetId> UserID, FName SessionName, const FOnlineSessionSearchResult& SearchResult);
	
	/*
	*	Overload of JoinFoundSession
	*	Used for joining a match off of a game invite
	*	
	*	@todo(chrisr): Support match invites, and joining off of steam friends
	*	
	*	@param LocalUser:
	*	@param SessionName:
	*	@param SearchResult:
	*	
	**/
	bool JoinFoundSession(int32 LocalUser, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	/*
	*
	*	@param SessionName:
	*	@param Result:
	*
	**/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/*
	 *	
	 *	@param Index:
	 *	
	 **/
	UFUNCTION(BlueprintCallable, Category = "Session Wrappers", meta = (DisplayName = "Join Session Wrapper"))
	void K2_JoinSession(int32 Index);
};