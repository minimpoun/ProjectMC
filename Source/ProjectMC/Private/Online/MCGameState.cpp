#include "MCGameState.h" 

AMCGameState::AMCGameState()
{

}

// @todo(chrisr): This is a really great meme, come back and actually do the shit
// inb4 this makes it to release.
// if this makes it to release i'm gonna die, just sayin.
void AMCGameState::SetupTeams(int32 NumOfTeams)
{
	TeamColors.Empty();
	TeamNames.Empty();

	if (NumOfTeams == 2)
	{
		TeamColors.Add(FColor::Red);
		TeamColors.Add(FColor::Blue);
		TeamNames.Add(TEXT("Red"));
		TeamNames.Add(TEXT("Blue"));
	}
	else if (NumOfTeams == 3)
	{
		TeamColors.Add(FColor::Red);
		TeamColors.Add(FColor::Blue);
		TeamColors.Add(FColor::Green);
		TeamNames.Add(TEXT("Red"));
		TeamNames.Add(TEXT("Blue"));
		TeamNames.Add(TEXT("Green"));
	}
	else
	{
		return;
	}
}
