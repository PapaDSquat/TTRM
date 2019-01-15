// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Utils/ObjectBag.h"
#include "../Actors/Block.h"
#include "Board.generated.h"

class ABlock;
struct FBlockTheme;
class ATetromino;
class ATetrisGameMode;
enum class ETetrominoType : int8;
class APlayerPawn;

UCLASS()
class TETRIS_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ResetBoard();
	void StopGame();

	// Input Interface
	void SetPaused(bool paused);
	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void RotateCW();
	void RotateCCW();
	void Drop();
	void Hold();

	// Events
	DECLARE_EVENT(ABoard, FPlaceTetrominoEvent)
	FPlaceTetrominoEvent& OnPlaceTetromino() { return m_evtPlaceTetromino; }

	DECLARE_EVENT_OneParam(ABoard, FClearLines123Event, int8 /*numLines*/)
	FClearLines123Event& OnClearLines123() { return m_evtClearLines123; }

	DECLARE_EVENT(ABoard, FClearTetrisEvent)
	FClearTetrisEvent& OnClearTetris() { return m_evtClearTetris; }

	DECLARE_EVENT(ABoard, FGameOverEvent)
	FGameOverEvent& OnGameOver() { return m_evtGameOver; }

private:
	struct TileData
	{
		bool filled;
		ABlock* block;
	};

	APlayerPawn* GetOwnerPawn();

	void OnDescendTimer();

	void SetActivePosition(const FIntPoint& newPosition);
	void RepositionActiveTetromino();
	void SpawnNewTetromino();

	// Return true if move is successful
	bool TryMoveTetromino(const FIntPoint& offset);

	void PlaceBlocks(const TArray< FIntPoint >& positions );
	void PlaceBlocks(const FIntPoint& position);

	void CopyTile(const TileData& source, TileData& dest) const;
	void SetTileFilled(uint8 row, uint8 col, bool filled);

	FBox2D GetActiveBounds() const;

	void UpdateGhost();

	void ResetDropTimer();

	ETetrominoType GetRandomTetrominoType();
	const FBlockTheme& GetRandomBlockTheme();

	static const uint8 s_gridRows = 24;
	static const uint8 s_gridCols = 10;
	TileData m_grid[s_gridRows][s_gridCols];

	ObjectBag< ETetrominoType > m_typeBag;
	ObjectBag< FBlockTheme > m_themeBag;

	ATetromino* m_activeTetromino;
	ATetromino* m_nextTetromino;
	ATetromino* m_holdTetromino;
	ATetromino* m_ghostTetromino;

	FIntPoint m_activePosition;
	FIntPoint m_ghostPosition;

	FTimerHandle m_dropTimerHandle;

	ATetrisGameMode* m_gameMode;

	bool m_canHold;

	// Events
	FPlaceTetrominoEvent m_evtPlaceTetromino;
	FClearLines123Event m_evtClearLines123;
	FClearTetrisEvent m_evtClearTetris;
	FGameOverEvent m_evtGameOver;

public:
	UPROPERTY(EditAnywhere, Category = "Tetris Setup")
	TSubclassOf<ATetromino> TetrominoClass;

	UPROPERTY(EditAnywhere, Category = "Tetris Setup")
	TSubclassOf<ABlock> BlockClass;
};
