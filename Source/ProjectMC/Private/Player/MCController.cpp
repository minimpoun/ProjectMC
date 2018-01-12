#include "MCController.h"
#include "Net/UnrealNetwork.h"
#include "MCPlayerState.h"
#include "MCGameInstance.h"
#include "MCCharacter.h"
#include "MCHUD.h"

// Removed these since we won't be doing in-game achievements as of right now.
// #define ACH_LOTSOFKILLS TEXT("LOTSOFKILLS")
// #define ACH_GET_KILL	TEXT("GETKILL")
// #define ACH_GOOD_SCORE	TEXT("GOODSCORE")
// #define ACH_GREAT_SCORE TEXT("GREATSCORE")
// 
// static const int32 LotsOfKills = 10;
// static const int32 GoodScore = 20;
// static const int32 GreatScore = 40;

AMCController::AMCController(const FObjectInitializer& ObjectInitializer)
{

}

void AMCController::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle RequestPlayerListUpdate;
	GetWorld()->GetTimerManager().SetTimer(RequestPlayerListUpdate, this, &AMCController::RequestPlayerListUpdate, 0.05f, false);

	if (UMCGameInstance* GameInstance = Cast<UMCGameInstance>(GetGameInstance()))
	{
		GameInstance->GamesparksObject->OnGameSparksAvailableDelegate.AddDynamic(this, &AMCController::OnGameSparksAvailable);
		GameInstance->GamesparksObject->Disconnect();
		GameInstance->GamesparksObject->Connect("k335910Ynt07", "iUvoG5ernS3bTv99sub6X7vW8KpGLpSJ");
	}
}

void AMCController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ShowScoreboard", IE_Pressed, this, &AMCController::OnToggleScoreboard);
	InputComponent->BindAction("ShowScoreboard", IE_Released, this, &AMCController::OnHideScoreboard);
}

void AMCController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMCController::OnGameSparksAvailable(bool available)
{
	if (available)
	{
		GS_Global& gs = UGameSparksModule::GetModulePtr()->GetGSInstance();
		GS_AuthRequest ar(gs);

		ar.Send(AuthenticationRequest_Response);
	}
}

void AMCController::AuthenticationRequest_Response(GS_Global& GS_Globals, const GS_AuthResponse& GS_AuthResponse)
{
	if (!GS_AuthResponse.GetHasErrors())
	{
		GS_Global& gs = UGameSparksModule::GetModulePtr()->GetGSInstance();
		GS_AccountDetails AccountDetailsRequest(gs);

		AccountDetailsRequest.Send();
	}
}

FORCEINLINE AMCHUD* AMCController::GetMCHUD() const
{
	return Cast<AMCHUD>(GetHUD());
}

void AMCController::OnBroadcastDeathMessage(class AMCPlayerState* KillerPS, class AMCPlayerState* VictimPS)
{
	AMCHUD* HUD = GetMCHUD();
	if (HUD)
	{
		HUD->DisplayKillToast(KillerPS, VictimPS);
	}

	ULocalPlayer* LP = Cast<ULocalPlayer>(Player);
	if (LP->GetCachedUniqueNetId().IsValid() && VictimPS->UniqueId.IsValid() && LP)
	{
		if (*LP->GetCachedUniqueNetId() == *VictimPS->UniqueId)
		{
			// @todo(chrisr, devlinw): Went ahead and added the support to do custom logic for the player that died. Not sure what we'd want to do tho
		}
	}

	// Refer to Header file
	// UpdateAchievementProgress(ACH_GET_KILL, 100.f);

	//////////////////////////////////////////////////////////////////////////
	// @removed(chrisr, 79): removed this due to probably never using custom event interfaces. If we do anything like this it'll be done through GameSparks
// 	const auto OnlineIdentity = Online::GetIdentityInterface();
// 	const auto OnlineEvent = Online::GetEventsInterface();
// 
// 	if (OnlineEvent.IsValid() && OnlineIdentity.IsValid())
// 	{
// 		ULocalPlayer* LP = Cast<ULocalPlayer>(Player);
// 		if (LP)
// 		{
// 			int32 UserIndex = LP->GetControllerId();
// 			TSharedPtr<const FUniqueNetId> UserID = OnlineIdentity->GetUniquePlayerId(UserIndex);
// 			if (UserID.IsValid())
// 			{
// 				if (AMCCharacter* Character = Cast<AMCCharacter>(GetCharacter()))
// 				{
// 					FVector Loc = Character ? Character->GetActorLocation() : LastDeathLoc;
// 
// 					FOnlineEventParms EventParams;
// 
// 					EventParams.Add(TEXT("GameplayModeId"), FVariantData((int32)1)); // @todo(chrisr): determine which game mode
// 					EventParams.Add(TEXT("LocationX"), FVariantData(Loc.X));
// 					EventParams.Add(TEXT("LocationY"), FVariantData(Loc.Y));
// 					EventParams.Add(TEXT("LocationZ"), FVariantData(Loc.Z));
// 
// 					OnlineEvent->TriggerEvent(*UserID, TEXT("KillSomeNerd"), EventParams);
// 				}
// 			}
// 		}
// 	}
}

void AMCController::ToggleHUD()
{
	
}

void AMCController::OnToggleScoreboard()
{

}

void AMCController::OnHideScoreboard()
{

}

// Refer to Header file
// void AMCController::QueryAchievementList()
// {
// 	ULocalPlayer* LP = Cast<ULocalPlayer>(Player);
// 	if (LP && LP->GetControllerId() != -1)
// 	{
// 		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
// 		if (OnlineSubsystem)
// 		{
// 			IOnlineIdentityPtr OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
// 			if (OnlineIdentity.IsValid())
// 			{
// 				TSharedPtr<const FUniqueNetId> UserID = OnlineIdentity->GetUniquePlayerId(LP->GetControllerId());
// 				if (UserID.IsValid())
// 				{
// 					IOnlineAchievementsPtr OnlineAchievements = OnlineSubsystem->GetAchievementsInterface();
// 					if (OnlineAchievements.IsValid())
// 					{
// 						OnlineAchievements->QueryAchievements(*UserID.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &AMCController::OnQueryAchievementsComplete));
// 					}
// 				} //@todo(chrisr): Logging
// 			}
// 		}
// 	}
// }

// Refer to Header file
// void AMCController::OnQueryAchievementsComplete(const FUniqueNetId& UserID, const bool bWasSUCCessful)
// {
// 	//@todo(chrisr):
// }

// Refer to Header file
// void AMCController::UpdateAchievementProgress(const FString& AchievementID, float PrecentToUpdate)
// {
// 	ULocalPlayer* LP = Cast<ULocalPlayer>(Player);
// 	if (LP)
// 	{
// 		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
// 		if (OnlineSubsystem)
// 		{
// 			IOnlineIdentityPtr OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
// 			if (OnlineIdentity.IsValid())
// 			{
// 				TSharedPtr<const FUniqueNetId> UserID = LP->GetCachedUniqueNetId();
// 				if (UserID.IsValid())
// 				{
// 					IOnlineAchievementsPtr OnlineAchievements = OnlineSubsystem->GetAchievementsInterface();
// 					if ((!WriteObjectPtr.IsValid() || WriteObjectPtr->WriteState != EOnlineAsyncTaskState::InProgress) && OnlineAchievements.IsValid())
// 					{
// 						WriteObjectPtr = MakeShareable(new FOnlineAchievementsWrite());
// 						WriteObjectPtr->SetFloatStat(*AchievementID, PrecentToUpdate);
// 					}
// 				} //@todo(chrisr): Logging
// 			}
// 		}
// 	}
// }

bool AMCController::CanGameStart() const
{
	if (Role == ROLE_Authority)
	{
		AMCBaseGamemode* GM = Cast<AMCBaseGamemode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			return GM->GetAllPlayersReady();
		}
	}
	return false;
}

void AMCController::StartGame()
{
	if (Role == ROLE_Authority)
	{
		AMCBaseGamemode* GM = Cast<AMCBaseGamemode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->StartGameFromLobby();
		}
	}
}

void AMCController::SendMessage(const FString& ChatMessage)
{
	if (Role == ROLE_Authority)
	{
		AMCBaseGamemode* GM = Cast<AMCBaseGamemode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			const FString OutChatMessage = FString(PlayerState->PlayerName + ": " + ChatMessage);
			GM->BroadcastMessage(OutChatMessage);
		}
	}
	else
	{
		ServerSendMessage(ChatMessage);
	}
}

void AMCController::ServerSendMessage_Implementation(const FString& ChatMessage)
{
	SendMessage(ChatMessage);
}

void AMCController::ClientReceviedMessage_Implementation(const FString& ChatMessage)
{
	ReceiveChatMessage(ChatMessage);
}

void AMCController::ClientUpdatePlayerList_Implementation(const TArray<FPlayerInfo>& ConnectedPlayerInfo)
{
	UpdatePlayerList(ConnectedPlayerInfo);
}

void AMCController::RequestPlayerListUpdate()
{
	if (Role == ROLE_Authority)
	{
		AMCBaseGamemode* GM = Cast<AMCBaseGamemode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->PlayerRequestUpdate();
		}
	}
	else
	{
		ServerRequestPlayerListUpdate();
	}
}

void AMCController::ServerRequestPlayerListUpdate_Implementation()
{
	RequestPlayerListUpdate();
}

void AMCController::SetReadyState(bool bNewReadyState)
{
	if (Role == ROLE_Authority)
	{
		AMCPlayerState* PS = Cast<AMCPlayerState>(PlayerState);
		if (PS)
		{
			PS->bIsReady = bNewReadyState;
			ServerRequestPlayerListUpdate();
		}
	}
	else
	{
		ServerSetReadyState(bNewReadyState);
	}
}

void AMCController::ServerSetReadyState_Implementation(bool bNewReadyState)
{
	SetReadyState(bNewReadyState);
}