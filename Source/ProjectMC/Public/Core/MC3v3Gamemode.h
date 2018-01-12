#pragma once

#include "CoreMinimal.h"
#include "MCBaseGamemode.h"

#include "MC3v3Gamemode.generated.h"

UCLASS()
class PROJECTMC_API AMC3v3Gamemode : public AMCBaseGamemode
{
	GENERATED_BODY()
	
private:

	// Public Variables
public:

	// Public Functions
public:

	AMC3v3Gamemode();

	FORCEINLINE int32 GetPlayersPerTeam() const
	{
		return 0;
	}


protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void PreInitializeComponents() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void InitGameState() override;

};