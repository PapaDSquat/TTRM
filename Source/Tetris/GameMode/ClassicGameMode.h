// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisGameMode.h"
#include "ClassicGameMode.generated.h"

class APlayerPawn;

/**
 * 
 */
UCLASS()
class TETRIS_API AClassicGameMode : public ATetrisGameMode
{
	GENERATED_BODY()
	
public:
	AClassicGameMode();

	// AGameModeBase
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	
	// ATetrisGameMode
	virtual bool OnClearLines(APlayerPawn* playerPawn, uint8 numLines) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Tetris")
	virtual float GetGameSpeed() const override;
	virtual float GetTetrominoDropTime() const;

	UPROPERTY(BlueprintReadOnly, Category = "Tetris")
	int32 Level;

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	int32 GetLevel() const { return Level; }
	
	//== EVENTS
	// TODO: Change to a struct of score data
	DECLARE_EVENT_TwoParams(ABoard, FScoreEvent, int32, int32)
	FScoreEvent& OnScoreEvent() { return m_scoreEvent; }

private:
	FScoreEvent m_scoreEvent;

};
