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
	// Use same scoring for now
	return Super::OnClearLines(playerPawn, numLines);
}

void AAttackGameMode::OnBoardPlaceTetromino(ABoard* sourceBoard, int8 numLinesCleared)
{
	if (!sourceBoard)
		return;

	APlayerPawn* sourcePawn = sourceBoard->GetOwnerPawn();
	if (!sourcePawn)
		return;

	FRoundData& sourceRound = GetRoundData(sourceBoard);

	APlayerPawn* attackingPawn = sourceRound.AttackingPlayer;
	const bool hasQueuedLines = sourceRound.QueuedLines > 0 && attackingPawn;

	// Simple block place, no lines were cleared.
	// If the player has queued lines, then fire them on self, and clear the queue
	if (numLinesCleared == 0)
	{
		if(hasQueuedLines)
		{
			sourceBoard->SendLines(sourceRound.QueuedLines);

			// TODO: Score to attacking player
			sourceRound.QueuedLines = 0;
			sourceRound.AttackingPlayer = nullptr;
		}
	}
	else
	{
		// Determine number of lines to send from number of lines cleared
		int32 linesToSend = 0;
		switch (numLinesCleared)
		{
		//   1: send nothing
		case 2: linesToSend = 1; break;
		case 3: linesToSend = 2; break;
		case 4: linesToSend = 4; break;
		}

		if (linesToSend > 0)
		{
			// If the player has queued lines, try to reduce the queue (garbage canceling).
			// As long as the player keeps clearing lines, they will hold off the queue (garbage blocking).
			// If the queue goes into negatives, the attack gets reversed back to the attacker.
			if (hasQueuedLines)
			{
				const int32 newQueue = sourceRound.QueuedLines - linesToSend;
				sourceRound.QueuedLines = FMath::Max(0, newQueue);
				if (newQueue < 0)
				{
					// Send attack back to attacker
					QueueAttack(sourcePawn, attackingPawn, FMath::Abs(newQueue));
				}
			}
			// If the queue is empty, simply attack a random target
			else
			{
				for (auto it = GetWorld()->GetPawnIterator(); it; ++it)
				{
					APlayerPawn* targetPawn = Cast< APlayerPawn >(it->Get());
					if (targetPawn != sourcePawn)
					{
						// todo: Random player
						QueueAttack(sourcePawn, targetPawn, linesToSend);
						break;
					}
				}
			}
		}
	}
}

void AAttackGameMode::QueueAttack(APlayerPawn* source, APlayerPawn* target, int32 linesToSend)
{
	FRoundData& targetRound = GetRoundData(target);
	targetRound.AttackingPlayer = source;
	targetRound.QueuedLines = linesToSend;
}

AAttackGameMode::FRoundData& AAttackGameMode::GetRoundData(ABoard* board) const
{
	return GetRoundData(board->GetOwnerPawn());
}

AAttackGameMode::FRoundData& AAttackGameMode::GetRoundData(APlayerPawn* pawn) const
{
	return m_roundData[pawn];
}
