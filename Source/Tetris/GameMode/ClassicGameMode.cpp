// Fill out your copyright notice in the Description page of Project Settings.

#include "ClassicGameMode.h"
#include "../Audio/TetrisAudioManager.h"
#include "../Theme/TetrisTheme.h"
#include "../Player/TetrisPlayerState.h"
#include "../Player/PlayerPawn.h"
#include "../TetrisGameInstance.h"

static const int32 s_linesPerLevel = 10;

AClassicGameMode::AClassicGameMode()
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
	if (!playerPawn || numLines == 0)
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

	FPlayerRoundStats& stats = playerPawn->GetRoundStats();

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

float AClassicGameMode::GetTetrominoDropTime(APlayerPawn* playerPawn) const
{
	if (!playerPawn) return Super::GetTetrominoDropTime(playerPawn);

	// Source: https://gaming.stackexchange.com/a/13129
	const int32 level = GetPlayerLevel(playerPawn);
	switch (level)
	{
	case 0: return 0.8f;
	case 1: return 0.72f;
	case 2: return 0.63f;
	case 3: return 0.55f;
	case 4: return 0.47f;
	case 5: return 0.38f;
	case 6: return 0.3f;
	case 7: return 0.22f;
	case 8: return 0.13f;
	case 9: return 0.1f;
	case 10: case 11: case 12: return 0.08f;
	case 13: case 14: case 15: return 0.07f;
	case 16: case 17: case 18: return 0.05f;
	default:
		if (level >= 19 && level <= 28) return 0.03f;
		else if (level >= 29) return 0.02f;
	}
	check(false && "How the hell did we even get here? What does it all mean?");
	return Super::GetTetrominoDropTime(playerPawn);
}

