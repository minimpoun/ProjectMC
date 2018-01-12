#include "MCBaseGamemode.h"
#include "Net/UnrealNetwork.h"
#include "MCController.h"
#include "MCPlayerState.h"
#include "UnrealMathUtility.h"
#include "MCTeamManager.h"
#include "Engine/DataTable.h"
#include "ConstructorHelpers.h"
#include "MCTeamManager.h"
#include "MCGameState.h"

AMCBaseGamemode::AMCBaseGamemode()
{
	ConstructorHelpers::FObjectFinder<UDataTable> TeamDT(TEXT("/Game/Types/DT_TeamData"));
	if (TeamDT.Object)
	{
		TeamDataTable = TeamDT.Object;
	}
}

void AMCBaseGamemode::NotifyKills(AController* Killer, AController* KilledPlayer, APawn* KilledPawn, const FGameplayEffectSpec& KillingEffect)
{
	AMCPlayerState* KillerPS = Killer ? Cast<AMCPlayerState>(Killer->PlayerState) : nullptr;
	AMCPlayerState* VictimPS = KilledPlayer ? Cast<AMCPlayerState>(KilledPlayer->PlayerState) : nullptr;

	if (KillerPS && KillerPS != VictimPS) // @note(chrisr): Don't score a kill/display kill notifications to other players if it's a suicide. We might want to change this later
	{
		KillerPS->AddKill(VictimPS, ScorePerKill);
		KillerPS->AnnounceKill(KillerPS, VictimPS);
	}
	if (VictimPS)
	{
		VictimPS->AddDeath(VictimPS, 1);
		VictimPS->AnnounceDeath(KillerPS, VictimPS);
	}
}

void AMCBaseGamemode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	
}

void AMCBaseGamemode::BeginPlay()
{
	Super::BeginPlay();
}

// void AMCBaseGamemode::KilledBy(AController* Killer, AActor* DamageCauser, AController* KilledController, AActor* KilledActor)
// {
// 	if (KilledController)
// 	{
// 		KilledController->ChangeState(NAME_Spectating);
// 		RestartPlayer(KilledController);
// 	}
// }

void AMCBaseGamemode::BroadcastMessage(const FString& ChatMessage)
{
	for (AMCController* Controller : ConnectedPlayers)
	{
		Controller->ClientReceviedMessage(ChatMessage);
	}
}

void AMCBaseGamemode::PlayerRequestUpdate()
{
	UpdatePlayerList();
}

void AMCBaseGamemode::UpdatePlayerList()
{
	PlayerInfoArray.Empty();

	for (AMCController* Controller : ConnectedPlayers)
	{
		FPlayerInfo TempPlayerInfo;
		
		if (AMCPlayerState* PS = Cast<AMCPlayerState>(Controller->PlayerState))
		{
			TempPlayerInfo.bIsReady = PS->bIsReady;
		}
		else
		{
			TempPlayerInfo.bIsReady = false;
		}

		TempPlayerInfo.PlayerName = Controller->PlayerState->PlayerName;
		PlayerInfoArray.Add(TempPlayerInfo);
	}
	for (AMCController* Controller : ConnectedPlayers)
	{
		Controller->ClientUpdatePlayerList(PlayerInfoArray);
	}
}

void AMCBaseGamemode::StartGameFromLobby()
{
	GetWorld()->ServerTravel(GameMap);
}

bool AMCBaseGamemode::GetAllPlayersReady() const
{
	for (AMCController* Controller : ConnectedPlayers)
	{
		if (AMCPlayerState* PS = Cast<AMCPlayerState>(Controller->PlayerState))
		{
			if (!PS->bIsReady)
			{
				return false;
			}
		}
	}
	return true;
}

bool AMCBaseGamemode::IsTeamFull()
{
	return true;
}

FString AMCBaseGamemode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal /*= TEXT("")*/)
{
	Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	return "";
}

APlayerController* AMCBaseGamemode::ProcessClientTravel(FString& URL, FGuid NextMapGuid, bool bSeamless, bool bAbsolute)
{
	Super::ProcessClientTravel(URL, NextMapGuid, bSeamless, bAbsolute);

	return nullptr;
}

void AMCBaseGamemode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AMCBaseGamemode::InitGameState()
{
	Super::InitGameState();
}

int32 AMCBaseGamemode::GetNumPlayers()
{
	Super::GetNumPlayers();

	return 0;
}

void AMCBaseGamemode::StartPlay()
{
	Super::StartPlay();
}

void AMCBaseGamemode::ResetLevel()
{
	Super::ResetLevel();
}

bool AMCBaseGamemode::CanServerTravel(const FString& URL, bool bAbsolute)
{
	Super::CanServerTravel(URL, bAbsolute);
	
	return true;
}

void AMCBaseGamemode::ProcessServerTravel(const FString& URL, bool bAbsolute /*= false*/)
{
	Super::ProcessServerTravel(URL, bAbsolute);
}

void AMCBaseGamemode::GameWelcomePlayer(UNetConnection* Connection, FString& RedirectURL)
{
	Super::GameWelcomePlayer(Connection, RedirectURL);
}

void AMCBaseGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AMCController* JoiningController = Cast<AMCController>(NewPlayer))
	{
		ConnectedPlayers.Add(JoiningController);
		GetPlayerState(NewPlayer->PlayerState);
	}
}

void AMCBaseGamemode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (AMCController* LeavingController = Cast<AMCController>(Exiting))
	{
		ConnectedPlayers.Remove(LeavingController);
		UpdatePlayerList();
	}
}

void AMCBaseGamemode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}

void AMCBaseGamemode::SetPlayerDefaults(APawn* PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);
}

void AMCBaseGamemode::Reset()
{
	Super::Reset();
}

void AMCBaseGamemode::PostLoad()
{
	Super::PostLoad();
}

AMCPlayerState* AMCBaseGamemode::GetPlayerState(APlayerState* NewPS)
{
	AMCPlayerState* PS = Cast<AMCPlayerState>(NewPS);
	check(PS);
	return PS;
}

void AMCBaseGamemode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}