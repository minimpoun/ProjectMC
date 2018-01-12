#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"

#include "MCPlayerStart.generated.h"

UCLASS()
class PROJECTMC_API AMCPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category  = Defaults)
	uint8 TeamID;
};