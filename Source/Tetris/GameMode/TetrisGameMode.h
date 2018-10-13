// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "../Tetris/Theme/TetrisTheme.h"
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

	FTetrisTheme m_currentTheme;

public:
	UPROPERTY(EditAnywhere, Category = "Tetris")
	int32 InitialGameSpeed;

	UPROPERTY(EditAnywhere, Category = "Tetris")
	int32 InitialTetrominoDropTime;

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	const FTetrisTheme& GetCurrentTheme() const;

	// Returns true if theme was changed successfully
	UFUNCTION(BlueprintCallable, Category = "Tetris")
	bool SetTheme(const FName& themeName);

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	const TArray<FTetrisTheme>& GetAllThemes() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris")
	UTetrisThemeCollection* Themes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris")
	FName ThemeID;

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

	// Events
public:
	DECLARE_EVENT_OneParam(ABoard, FSetThemeEvent, const FTetrisTheme&)
	FSetThemeEvent& OnSetTheme() { return m_evtSetTheme; }

private:
	FSetThemeEvent m_evtSetTheme;
};

	
