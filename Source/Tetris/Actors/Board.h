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

USTRUCT(BlueprintType, Category = "Tetris Debug")
struct FDebugTableRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Tetris")
	TArray<bool> Cols;
};
using FDebugTable = TArray< FDebugTableRow >;

UCLASS(BlueprintType)
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

	APlayerPawn* GetOwnerPawn();
	void SendLines(int32 numLines);

	// Input Interface
	void SetPaused(bool paused);
	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void RotateCW();
	void RotateCCW();
	void Drop();
	void Hold();

	// Helpers
	static uint8 GetTotalRows() { return s_gridRows; }
	static uint8 GetTotalColumns() { return s_gridCols; }

	// Events
	DECLARE_EVENT_TwoParams(ABoard, FPlaceTetrominoEvent, ABoard* /*board*/, int8 /*numLines*/)
	FPlaceTetrominoEvent& OnPlaceTetromino() { return m_evtPlaceTetromino; }

	DECLARE_EVENT_TwoParams(ABoard, FClearLines123Event, ABoard* /*board*/, int8 /*numLines*/)
	FClearLines123Event& OnClearLines123() { return m_evtClearLines123; }

	DECLARE_EVENT_OneParam(ABoard, FClearTetrisEvent, ABoard* /*board*/ )
	FClearTetrisEvent& OnClearTetris() { return m_evtClearTetris; }

	DECLARE_EVENT_OneParam(ABoard, FGameOverEvent, ABoard* /*board*/ )
	FGameOverEvent& OnGameOver() { return m_evtGameOver; }

private:
	struct TileData
	{
		bool filled;
		ABlock* block;
	};

	void OnDescendTimer();

	void SetActivePosition(const FIntPoint& newPosition);
	void ResetActivePosition();
	void RepositionActiveTetromino();
	void SpawnNewTetromino();

	// Return true if move is successful
	bool TryMoveTetromino(const FIntPoint& offset);

	void PlaceBlocks(const TArray< FIntPoint >& positions );
	void PlaceBlocks(const FIntPoint& position);

	void CopyTile(const TileData& source, TileData& dest) const;
	void SetBlockFilled(uint8 row, uint8 col, bool filled);
	void SetBlockAsGarbage(uint8 row, uint8 col);
	static bool IsValidGridPosition(uint8 row, uint8 col);

	FBox2D GetActiveBounds() const;
	bool GetBlockDefaultFill(uint8 row, uint8 col) const;

	void UpdateGhost();

	void ResetDescendTimer();

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

	FTimerHandle m_descendTimerHandle;

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

	// Debug
	UPROPERTY(EditAnywhere, Category = "Tetris Debug")
	bool DEBUG_DefaultBoardEnabled;

	//using DebugRow = TArray<bool, TFixedAllocator<s_gridRows>>;
	//using DebugTable = TArray< DebugRow, TFixedAllocator<s_gridCols>>;
	UPROPERTY(EditAnywhere, Category = "Tetris Debug")
	TArray< FDebugTableRow > DEBUG_DefaultBoard;
};
