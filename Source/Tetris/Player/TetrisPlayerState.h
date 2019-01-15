// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TetrisPlayerState.generated.h"

USTRUCT(BlueprintType)
struct FPlayerRoundStats
{
	GENERATED_USTRUCT_BODY()

public:
	FPlayerRoundStats()
	{}

	// static const FScoreData& DEFAULT();

	UPROPERTY(BlueprintReadOnly, Category = "Tetris")
	int32 Score;

	UPROPERTY(BlueprintReadOnly, Category = "Tetris")
	int32 Lines;

	UPROPERTY(BlueprintReadOnly, Category = "Tetris")
	int32 Level;
};

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATetrisPlayerState();

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	FPlayerRoundStats& GetRoundStats();
	
private:
	FPlayerRoundStats m_roundStats;
	
};
