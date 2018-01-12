#include "MCHUD.h"
#include "MCBaseGamemode.h"
#include "MCGameState.h"
#include "MCPlayerState.h"

void AMCHUD::DisplayKillToast(class AMCPlayerState* KillerPS, class AMCPlayerState* VictimPS)
{
	AMCGameState* GS = Cast<AMCGameState>(GetWorld()->GetGameState());
	if (GS)
	{
		const AMCBaseGamemode* GM = GS->GetDefaultGameMode<AMCBaseGamemode>();
		AMCPlayerState* PS = PlayerOwner ? Cast<AMCPlayerState>(PlayerOwner->PlayerState) : nullptr;
		if (GM && PS && KillerPS && VictimPS)
		{
			FMessageContainer NewMessageContainer;

			NewMessageContainer.KillerName = KillerPS->GetShortendPlayerName();
			NewMessageContainer.VictimName = VictimPS->GetShortendPlayerName();
			NewMessageContainer.KillerTeamID = KillerPS->GetTeam();
			NewMessageContainer.VictimTeamID = VictimPS->GetTeam();

			ActiveMessages.Add(NewMessageContainer);
			if (KillerPS == PS && VictimPS != PS)
			{
				KilledPlayerMessage = FText::FromString(NewMessageContainer.VictimName);
			}
			OnCreateKillFeedMessage(NewMessageContainer, MessageDurationInSeconds);
		}
	}
}

void AMCHUD::BeginPlay()
{
	
}	

void AMCHUD::DrawDeathMessage()
{
	
}

FColor AMCHUD::GetTeamColorFromID(const uint8 TeamID)
{
	AMCGameState* GS = GetWorld()->GetGameState() ? Cast<AMCGameState>(GetWorld()->GetGameState()) : nullptr;
	if (GS)
	{
		return GS->TeamColors[TeamID];
	}
	else
	{
		return FColor::Purple;
	}
}

FString AMCHUD::GetTeamNameFromID(const uint8 TeamID)
{
	AMCGameState* GS = GetWorld()->GetGameState() ? Cast<AMCGameState>(GetWorld()->GetGameState()) : nullptr;
	if (GS)
	{
		return GS->TeamNames[TeamID];
	}
	else
	{
		return "Invalid Team";
	}
}
