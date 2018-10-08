// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisGameMode.generated.h"

struct FTetrisTheme;

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
	UPROPERTY(EditAnywhere, Category = "Tetris")
	int32 InitialGameSpeed;

	UPROPERTY(EditAnywhere, Category = "Tetris")
	int32 InitialTetrominoDropTime;

	// TODO: Find a way to expose this to BP
	const FTetrisTheme* GetCurrentTheme() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris")
	TSoftObjectPtr<UDataTable> ThemesData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris")
	FName ThemeName;

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	virtual float GetGameSpeed() const;
	virtual float GetTetrominoDropTime() const;

	// TODO: Multiple Players. Group stats into each.
	UPROPERTY(BlueprintReadOnly, Category = "Tetris")
	int32 Lines;
	UFUNCTION(BlueprintCallable, Category = "Tetris")
	int32 GetLines() const { return Lines; }

	UPROPERTY(BlueprintReadOnly, Category = "Tetris")
	int32 Score;
	UFUNCTION(BlueprintCallable, Category = "Tetris")
	int32 GetScore() const { return Score; }

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	FString GetRoundTimeString() const;
};

	
