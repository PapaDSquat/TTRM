// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClassicGameMode.h"
#include "AttackGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackQueuedEvent, int, NumLines, APlayerPawn*, Attacker, APlayerPawn*, Receiver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackFiredEvent, int, NumLines, APlayerPawn*, Attacker, APlayerPawn*, Receiver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQueueChangedEvent, int, NewQueueSize, APlayerPawn*, Source);

class ABoard;

UCLASS()
class TETRIS_API AAttackGameMode : public AClassicGameMode
{
	GENERATED_BODY()
	
public:
	// ATetrisGameMode
	virtual void OnStartGameInternal() override;
	virtual bool OnClearLines(APlayerPawn* playerPawn, uint8 numLines) override;

	// Board Callbacks
	void OnBoardPlaceTetromino(ABoard* sourceBoard, int8 numLinesCleared);

private:
	void QueueAttack(APlayerPawn* source, APlayerPawn* target, int32 linesToSend);
	bool TryClearAttack(APlayerPawn* source, int32 linesToClear);

	// TODO: Improve this
	struct FRoundData
	{
		int32 QueuedLines{ 0 };
		APlayerPawn* AttackingPlayer;
	};
	mutable TMap< APlayerPawn*, FRoundData > m_roundData;

public:
	FRoundData& GetRoundData(APlayerPawn* pawn) const;
	FRoundData& GetRoundData(ABoard* board) const;

	//== EVENTS
	UPROPERTY(BlueprintAssignable)
	FAttackQueuedEvent OnAttackQueued;
	
	UPROPERTY(BlueprintAssignable)
	FAttackFiredEvent OnAttackFired;

	UPROPERTY(BlueprintAssignable)
	FQueueChangedEvent OnQueueChanged;
};
