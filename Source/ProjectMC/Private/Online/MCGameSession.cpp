#include "MCGameSession.h"

void AMCGameSession::RegisterServer()
{
	Super::RegisterServer();
}

void AMCGameSession::RegisterServerFailed()
{
	Super::RegisterServerFailed();
}

bool AMCGameSession::KickPlayer(APlayerController* KickedPlayer, const FText& KickReason)
{
	bool no_fuck_you = Super::KickPlayer(KickedPlayer, KickReason);

	return no_fuck_you;
}