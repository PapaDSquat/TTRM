// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATetrisGameMode();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnClearLines(uint8 numLines) {}

protected:
	float m_startTime;
	float m_currentTime;

public:
	// TODO: Multiple Players. Group stats into each.
	UPROPERTY(BlueprintReadOnly, Category = "Tetris")
	int32 Lines;

	UPROPERTY(BlueprintReadOnly, Category = "Tetris")
	int32 Score;

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	int32 GetLines() const { return Lines; }

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	int32 GetScore() const { return Score; }

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	FString GetRoundTimeString() const;
};