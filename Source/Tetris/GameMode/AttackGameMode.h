// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClassicGameMode.h"
#include "AttackGameMode.generated.h"

class ABoard;

/**
 * 
 */
UCLASS()
class TETRIS_API AAttackGameMode : public AClassicGameMode
{
	GENERATED_BODY()
	
public:
	// ATetrisGameMode
	virtual void OnStartGameInternal() override;
	virtual bool OnClearLines(APlayerPawn* playerPawn, uint8 numLines) override;

	// Board Callbacks
	void OnBoardPlaceTetromino(ABoard* board, int8 numLines);

private:
	void QueueAttack(APlayerPawn* source, APlayerPawn* target, int32 linesToSend);
	bool TryClearAttack(APlayerPawn* source, int32 linesToClear);

	// TODO: Improve this shit
	struct FRoundData
	{
		int32 QueuedLines{ 0 };
		APlayerPawn* AttackingPlayer;
	};
	mutable TMap< APlayerPawn*, FRoundData > m_roundData;
	FRoundData& GetRoundData(APlayerPawn* pawn) const;
	FRoundData& GetRoundData(ABoard* board) const;
};
