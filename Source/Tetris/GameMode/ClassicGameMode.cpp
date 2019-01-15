// Fill out your copyright notice in the Description page of Project Settings.

#include "ClassicGameMode.h"
#include "../Audio/TetrisAudioManager.h"
#include "../Theme/TetrisTheme.h"
#include "../Player/TetrisPlayerState.h"
#include "../Player/PlayerPawn.h"
#include "../TetrisGameInstance.h"

static const int32 s_linesPerLevel = 10;

AClassicGameMode::AClassicGameMode()
	: Level( 0 )
{

}

void AClassicGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AClassicGameMode::InitGameState()
{
	Super::InitGameState();
	
}

bool AClassicGameMode::OnClearLines(APlayerPawn* playerPawn, uint8 numLines)
{
	if (numLines == 0)
		return false;

	ATetrisPlayerState* playerState = Cast<ATetrisPlayerState>(playerPawn->PlayerState);
	if (!playerState)
		return false;

	/* 
	Original Tetris scoring system from: http://tetris.wikia.com/wiki/Scoring
	# of lines	|	points
		1			  40
		2			 100
		3			 300
		4			1200
	Formula: (points for # of lines) * (Level + 1)
	*/

	FPlayerRoundStats& stats = playerState->GetRoundStats();

	int32 linePointsMultiplier = 0;
	switch (numLines)
	{
	case 1: linePointsMultiplier = 40; break;
	case 2: linePointsMultiplier = 100; break;
	case 3: linePointsMultiplier = 300; break;
	case 4: linePointsMultiplier = 1200; break;
	}
	stats.Lines += numLines;

	// TODO: Should the score be split if leveling up during it?
	const int32 scoreAdded = linePointsMultiplier * (stats.Level + 1);
	stats.Score += scoreAdded;

	const int32 newLevel = stats.Lines / s_linesPerLevel;
	if (newLevel > stats.Level)
	{
		++stats.Level;
		GetTetrisGameInstance()->GetAudioManager()->PlaySound(GetCurrentTheme().LevelUpSound);
		return true;
	}
	return false;
}

void AClassicGameMode::BeginPlay()
{
	Super::BeginPlay();
}

float AClassicGameMode::GetGameSpeed() const
{
	return Level + 1;
}

float AClassicGameMode::GetTetrominoDropTime() const
{
	return InitialTetrominoDropTime / (GetGameSpeed());
}

