// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UTetrisGameInstance;
class ATetrisGameMode;
struct FTetrisTheme;
class ABoard;
class UUserWidget;
struct FPlayerRoundStats;
enum class EGameEventType : uint8;

enum class EMoveDirection
{
	Left,
	Right,
	Down,
	MAX
};

UCLASS()
class TETRIS_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UTetrisGameInstance* GetTetrisGameInstance();
	bool IsGameActive() const;

	void SetupBoard();

	// Input Interface
	void MoveLeft();
	void MoveLeftHold();
	void MoveAxisHorizontal(float value);
	void MoveRight();
	void MoveRightHold();
	void MoveAxisVertical(float value);
	void MoveDown();
	void MoveDownHold();
	void RotateCW();
	void RotateCCW();
	void Drop();
	void Hold();
	bool CanAxisMove(EMoveDirection direction) const;

	// Event Callbacks
	void OnGameEvent(EGameEventType eventType);
	void OnBoardPlaceTetromino();
	void OnBoardClearLines123(int8 numLines);
	void OnBoardClearTetris();
	void OnBoardGameOver();

	ATetrisGameMode* m_gameMode;
	ABoard*	m_board;

	// Input Improvements
	float m_timeBeforeMoveH{ 0.f };
	float m_timeBeforeMoveV{ 0.f };
	bool m_canAxisDrop{ 0.f };
	EMoveDirection m_lastMoveDirection{ EMoveDirection::MAX };

public:
	FPlayerRoundStats& GetRoundStats() const;


	UPROPERTY(EditAnywhere, Category = "Tetris Actors")
	TSubclassOf<ABoard> BoardClass; // Fallback

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	ABoard* GetBoard() const { return m_board; }
};
