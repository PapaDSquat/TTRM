// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackGameMode.h"
#include "../Actors/Board.h"
#include "../Player/PlayerPawn.h"

void AAttackGameMode::OnStartGameInternal()
{
	m_roundData.Empty();
	for (APlayerPawn* player : m_players)
	{
		m_roundData.Add(player, FRoundData());
		if (ABoard* board = player->GetBoard())
		{
			board->OnPlaceTetromino().AddUObject(this, &AAttackGameMode::OnBoardPlaceTetromino);
		}
	}
}

bool AAttackGameMode::OnClearLines(APlayerPawn* playerPawn, uint8 numLines)
{
	// Bind to events
	if (!playerPawn || numLines == 0)
		return false;

	const bool result = Super::OnClearLines(playerPawn, numLines);

	int32 linesToSend = 0;
	switch (numLines)
	{
	// 1 line sends nothing
	case 2: linesToSend = 1; break;
	case 3: linesToSend = 2; break;
	case 4: linesToSend = 4; break;
	}

	// TODO: Queue lines until next block
	if (linesToSend > 0)
	{
		// Find another player
		for (auto it = GetWorld()->GetPawnIterator(); it; ++it)
		{
			APlayerPawn* targetPawn = Cast< APlayerPawn >(it->Get());
			if (targetPawn != playerPawn)
			{
				// todo: Random player
				QueueAttack(playerPawn, targetPawn, linesToSend);
				break;
			}
		}
	}

	return result;
}

void AAttackGameMode::QueueAttack(APlayerPawn* source, APlayerPawn* target, int32 linesToSend)
{
	FRoundData& targetRound = GetRoundData(target);
	targetRound.AttackingPlayer = source;
	targetRound.QueuedLines = linesToSend;
}

bool AAttackGameMode::TryClearAttack(APlayerPawn* source, int32 linesToClear)
{
	ABoard* sourceBoard = source->GetBoard();
	FRoundData& sourceRound = GetRoundData(sourceBoard);
	APlayerPawn* attackingPawn = sourceRound.AttackingPlayer;
	if (sourceRound.QueuedLines && attackingPawn)
	{
		const int32 newLines = sourceRound.QueuedLines - linesToClear;
		sourceRound.QueuedLines = FMath::Max(0, newLines);
		if (newLines <= 0)
		{
			// Clear the attack
			sourceRound.AttackingPlayer = nullptr;

			if (newLines < 0)
			{
				// Attack the player back
				QueueAttack(source, attackingPawn, FMath::Abs(newLines));
			}

		}
		return true;
	}
	return false;
}

void AAttackGameMode::OnBoardPlaceTetromino(ABoard* board, int8 numLines)
{
	//	If player clears any lines during the attack, they don't receive anything until next block place
	const bool cleared = TryClearAttack(board->GetOwnerPawn(), numLines);
	if (!cleared)
	{
		FRoundData& round = GetRoundData(board);
		APlayerPawn* attackingPlayer = round.AttackingPlayer;
		if (round.QueuedLines && attackingPlayer)
		{
			board->SendLines(round.QueuedLines);

			round.QueuedLines = 0;
			round.AttackingPlayer = nullptr;
			// TODO: Score to attacking player
		}
	}
}

AAttackGameMode::FRoundData& AAttackGameMode::GetRoundData(ABoard* board) const
{
	return GetRoundData(board->GetOwnerPawn());
}

AAttackGameMode::FRoundData& AAttackGameMode::GetRoundData(APlayerPawn* pawn) const
{
	return m_roundData[pawn];
}
