#include "MCPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "MCController.h"
#include "MCGameState.h"

void AMCPlayerState::SetTeamColor()
{

}

// @note(chrisr): We don't use this Victim ref here, remove? I am keeping it atm in case we want to use it for something
void AMCPlayerState::AddKill(AMCPlayerState* Victim, int32 NewKill)
{
	AddScore(NewKill); // @note(chrisr): we don't have a way to know what kind of point we are scoring as of now. Maybe a @todo?
	CurrentKills++;
}

// @note(chrisr): The signature is not being used. Remove?
void AMCPlayerState::AddDeath(AMCPlayerState* Killer, int32 NewDeath)
{
	CurrentDeaths++;
}

void AMCPlayerState::AddScore(float NewScore)
{
	AMCGameState* const GS = GetWorld()->GetGameState<AMCGameState>();
	if (GS && TeamID != TEAM_None && TeamID != TEAM_Invalid)
	{
		// @todo(chrisr): implement team scores that are seperate from player scores
	}
}

void AMCPlayerState::SetTeam(uint8 NewTeam)
{

}

void AMCPlayerState::UpdateQuitterStatus(bool Val)
{

}

void AMCPlayerState::AnnounceKill_Implementation(class AMCPlayerState* Killer, class AMCPlayerState* Victim)
{
	if (Killer->UniqueId.IsValid())
	{
		for (FConstPlayerControllerIterator Index = GetWorld()->GetPlayerControllerIterator(); Index; ++Index)
		{
			AMCController* PC = Cast<AMCController>(*Index);
			if ( PC && PC->IsLocalController())
			{
				ULocalPlayer* LP = Cast<ULocalPlayer>(PC->Player);
				TSharedPtr<const FUniqueNetId> ID = LP->GetCachedUniqueNetId();
				if (ID.IsValid() && *LP->GetCachedUniqueNetId() == *Killer->UniqueId)
				{
					// @todo(chrisr && devlinw): Do something when the player gets a kill?
					// Everything is setup already, just gotta figure out what to do
				}
			} 
		}
	} 
}

void AMCPlayerState::AnnounceDeath_Implementation(class AMCPlayerState* Killer, class AMCPlayerState* Victim)
{
	for (FConstPlayerControllerIterator PC = GetWorld()->GetPlayerControllerIterator(); PC; ++PC)
	{
		AMCController* LPC = Cast<AMCController>(*PC);
		if(LPC && LPC->IsLocalPlayerController())
		{
			LPC->OnBroadcastDeathMessage(Killer, Victim);
		}
	}
}

FString AMCPlayerState::GetShortendPlayerName() const
{
	if (PlayerName.Len() > MaxNameLength)
	{
		return PlayerName.Left(MaxNameLength) + "....";
	}
	else
	{
		return PlayerName;
	}
}

void AMCPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void AMCPlayerState::UnregisterPlayerWithSession()
{
	if (!bFromPreviousLevel)
	{
		Super::UnregisterPlayerWithSession();
	}
}

void AMCPlayerState::ClientInitialize(class AController* C)
{
	Super::ClientInitialize(C);
}

void AMCPlayerState::Reset()
{
	Super::Reset();

	if (!bFromPreviousLevel)
	{
		TeamID = TEAM_None;
	}

	bIsReady = false;
	bIsMatchQuitter = false;
	CurrentDeaths = 0;
	CurrentKills = 0;
	TotalScore = 0.f;
	TotalDamage = 0.f;
	TotalHealing = 0.f;
	TotalProtection = 0.f;
}

void AMCPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(AMCPlayerState, bIsReady, COND_SkipOwner);
	DOREPLIFETIME(AMCPlayerState, TeamID);
	DOREPLIFETIME(AMCPlayerState, CurrentKills);
	DOREPLIFETIME(AMCPlayerState, CurrentDeaths);
}

