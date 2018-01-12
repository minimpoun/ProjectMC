#pragma once


#include "GameFramework/GameSession.h"

#include "MCGameSession.generated.h"

UCLASS(Abstract)
class PROJECTMC_API AMCGameSession : public AGameSession
{
	GENERATED_BODY()

protected:

	virtual void RegisterServer() override;

	virtual void RegisterServerFailed() override;

	virtual bool KickPlayer(APlayerController* KickedPlayer, const FText& KickReason) override;
};