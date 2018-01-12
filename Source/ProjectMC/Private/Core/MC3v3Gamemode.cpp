#include "MC3v3Gamemode.h"
#include "UObject/ConstructorHelpers.h"
#include "MCPlayerState.h"
#include "MCController.h"
#include "MCCharacter.h"
#include "MCGameState.h"
#include "MCPlayerStart.h"

AMC3v3Gamemode::AMC3v3Gamemode()
{
	static ConstructorHelpers::FClassFinder<AMCCharacter> BPPlayerCharacter(TEXT("/Game/GameBlueprints/BP_Character.BP_Character_C"));
	if (BPPlayerCharacter.Class)
	{
		DefaultPawnClass = BPPlayerCharacter.Class;
	}

	static ConstructorHelpers::FClassFinder<AMCController> BPPlayerController(TEXT("/Game/GameBlueprints/BP_Controller.BP_Controller_C"));
	if (BPPlayerController.Class)
	{
		PlayerControllerClass = BPPlayerController.Class;
	}

	static ConstructorHelpers::FClassFinder<AMCPlayerState> BPPlayerState(TEXT("/Game/GameBlueprints/BP_PlayerState.BP_PlayerState_C"));
	if (BPPlayerState.Class)
	{
		PlayerStateClass = BPPlayerState.Class;
	}
}

void AMC3v3Gamemode::BeginPlay()
{
	Super::BeginPlay();
}

void AMC3v3Gamemode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMC3v3Gamemode::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMC3v3Gamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AMC3v3Gamemode::InitGameState()
{
	Super::InitGameState();

	AMCGameState* GState = GameState ? Cast<AMCGameState>(GameState) : nullptr;
	if (GState)
	{
		GState->SetupTeams(2);
	}
}