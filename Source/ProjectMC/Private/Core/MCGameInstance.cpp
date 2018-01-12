#include "MCGameInstance.h"
#include "MCController.h"

// @TODO(chrisr): Documentation

UMCGameInstance::UMCGameInstance()
{
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMCGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMCGameInstance::OnJoinSessionComplete);
}

void UMCGameInstance::Init()
{
	Super::Init();

	GamesparksMessageListener = NewObject<UGSMessageListenersObject>(this, UGSMessageListenersObject::StaticClass());
	GamesparksObject = NewObject<UGameSparksObject>(this, UGameSparksObject::StaticClass());
}

void UMCGameInstance::FindSession(TSharedPtr<const FUniqueNetId> UserID, bool bIsPresence, bool bIsLAN, int32 MaxSearchResults, bool bIsDedicated, float TimeoutInSeconds)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface();
		if (OnlineSession.IsValid() && UserID.IsValid())
		{
			SessionSearchSettings->bIsLanQuery = bIsLAN;
			SessionSearchSettings->MaxSearchResults = MaxSearchResults;
			SessionSearchSettings->PingBucketSize = 50;
			SessionSearchSettings->TimeoutInSeconds = TimeoutInSeconds;

			if (bIsDedicated)
			{
				SessionSearchSettings->QuerySettings.Set(SEARCH_DEDICATED_ONLY, bIsDedicated, EOnlineComparisonOp::Equals);
			}

			if (bIsPresence)
			{
				SessionSearchSettings->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			OnFindSessionsCompleteHandle = OnlineSession->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			SessionSearchSettings = MakeShareable(new FOnlineSessionSearch());
			TSharedRef<FOnlineSessionSearch> SessionSearchSettingsRef = SessionSearchSettings.ToSharedRef();
			OnlineSession->FindSessions(*UserID, SessionSearchSettingsRef);
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}

void UMCGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface();
		if (OnlineSession.IsValid())
		{
			OnlineSession->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteHandle);
			TArray<FSearchResultBPWrapper> SearchResultBPWrapper;

			if (SessionSearchSettings->SearchResults.Num() > 0)
			{
				for (int32 Index = 0; Index < SessionSearchSettings->SearchResults.Num(); ++Index)
				{
					FSearchResultBPWrapper L_SearchResultBPWrapper;

					L_SearchResultBPWrapper.bIsLAN = SessionSearchSettings->SearchResults[Index].Session.SessionSettings.bIsLANMatch;
					L_SearchResultBPWrapper.CurrentConnectedPlayers = SessionSearchSettings->SearchResults[Index].Session.SessionSettings.NumPublicConnections - SessionSearchSettings->SearchResults[Index].Session.NumOpenPublicConnections;
					L_SearchResultBPWrapper.MaxNumberOfPlayers = SessionSearchSettings->SearchResults[Index].Session.SessionSettings.NumPublicConnections;
					L_SearchResultBPWrapper.Ping = SessionSearchSettings->SearchResults[Index].PingInMs;

					SessionSearchSettings->SearchResults[Index].Session.SessionSettings.Get(SETTING_SERVER_NAME, L_SearchResultBPWrapper.SessionName);

					SearchResultBPWrapper.Add(L_SearchResultBPWrapper);
				}
			}
			K2_OnFoundSessionsComplete(SearchResultBPWrapper);
		}
	}
}

bool UMCGameInstance::JoinFoundSession(TSharedPtr<const FUniqueNetId> UserID, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bSuccessful = false;

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSession)
	{
		IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface();
		if (OnlineSession.IsValid() && UserID.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = OnlineSession->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			bSuccessful = OnlineSession->JoinSession(*UserID, SessionName, SearchResult);
		}
	}
	return bSuccessful;
}

bool UMCGameInstance::JoinFoundSession(int32 LocalUser, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bSuccessful = false;

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSession)
	{
		IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface();
		if (OnlineSession.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = OnlineSession->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			bSuccessful = OnlineSession->JoinSession(LocalUser, SessionName, SearchResult);
		}
	}
	return bSuccessful;
}

void UMCGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface();
		if(OnlineSession.IsValid())
		{
			OnlineSession->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			APlayerController* const PC = GetFirstLocalPlayerController();
			FString MapURL;
			if (PC && OnlineSession->GetResolvedConnectString(SessionName, MapURL))
			{
				PC->ClientTravel(MapURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

/************************************************************************/
/*                     Blueprint Session Wrappers                       */
/************************************************************************/

void UMCGameInstance::K2_FindSession(bool bIsPresence, bool bIsLAN, int32 MaxSearchResults, bool bIsDedicated, float TimeoutInSeconds)
{
	ULocalPlayer* const L_Player = GetFirstGamePlayer();

	FindSession(L_Player->GetPreferredUniqueNetId(), bIsPresence, bIsLAN, MaxSearchResults, bIsDedicated, TimeoutInSeconds);
}

void UMCGameInstance::K2_JoinSession(int32 Index)
{
	ULocalPlayer* const L_Player = GetFirstGamePlayer();

	FOnlineSessionSearchResult OnlineSessionSearchResult;
	OnlineSessionSearchResult = SessionSearchSettings->SearchResults[Index];

	JoinFoundSession(L_Player->GetPreferredUniqueNetId(), GameSessionName, OnlineSessionSearchResult);
}
