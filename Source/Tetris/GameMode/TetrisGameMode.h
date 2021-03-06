// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "../Tetris/Theme/TetrisTheme.h"
#include "TetrisGameMode.generated.h"

class APlayerPawn;
class UTetrisGameInstance;
struct FTetrisTheme;

UENUM()
enum class EGameEventType : uint8
{
	Start,
	End,
	GameOver,
	Restart,
	Pause,
	Unpause
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetThemeDelegate, const struct FTetrisTheme&, Theme);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStartEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameRestartEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEndEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGamePauseEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameUnPauseEvent);

UCLASS(BlueprintType)
class TETRIS_API ATetrisGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATetrisGameMode();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Returns true if leveled up
	virtual bool OnClearLines(APlayerPawn* playerPawn, uint8 numLines) { return false; }
	virtual float GetTetrominoDropTime(APlayerPawn* playerPawn) const;
	int32 GetPlayerLevel(APlayerPawn* playerPawn) const;

protected:
	UTetrisGameInstance* GetTetrisGameInstance();
	void FireGameEvent( EGameEventType eventType );

	// Override for custom game modes
	virtual void OnStartGameInternal() {}
	virtual void OnEndGameInternal() {}
	virtual void OnGameOverInternal() {}
	virtual void OnRestartGameInternal() {}
	virtual void OnPauseGameInternal() {}
	virtual void OnUnpauseGameInternal() {}

	bool m_gameStarted;
	bool m_gamePaused;
	float m_startTime;
	float m_currentTime;

	FTetrisTheme m_currentTheme;

	TArray< APlayerPawn* > m_players; // Is this safe?

public:
	UFUNCTION(BlueprintCallable, Category = "Tetris")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	void EndGame();

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	void GameOver();

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	void RestartGame();

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	void UnpauseGame();

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	bool IsGameStarted() const;

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	bool IsGamePaused() const;

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	const FTetrisTheme& GetCurrentTheme() const;

	// Returns true if theme was changed successfully
	UFUNCTION(BlueprintCallable, Category = "Tetris")
	bool SetTheme(const FName& themeID);
	bool SetTheme(const FTetrisTheme& theme, bool restart = true);

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	const TArray<FTetrisTheme>& GetAllThemes() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris")
	UTetrisThemeCollection* Themes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris")
	FName DefaultThemeID;

	UPROPERTY(BlueprintReadOnly, Category = "Tetris")
	int32 NumPlayers;
	UFUNCTION(BlueprintCallable, Category = "Tetris")
	void SetNumPlayers(int32 num);

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
	DECLARE_EVENT_OneParam(ATetrisGameMode, FGameEvent, EGameEventType)
	FGameEvent& OnGameEvent() { return m_evtGameEvent; }

	UPROPERTY(BlueprintAssignable)
	FGameStartEvent OnGameStart;

	UPROPERTY(BlueprintAssignable)
	FGameRestartEvent OnGameRestart;

	UPROPERTY(BlueprintAssignable)
	FGameEndEvent OnGameEnd;

	UPROPERTY(BlueprintAssignable)
	FGameOverEvent OnGameOver;

	UPROPERTY(BlueprintAssignable)
	FGamePauseEvent OnGamePause;

	UPROPERTY(BlueprintAssignable)
	FGamePauseEvent OnGameUnPause;

	UPROPERTY(BlueprintAssignable)
	FSetThemeDelegate OnThemeChange;

private:
	FGameEvent m_evtGameEvent;

};

	
