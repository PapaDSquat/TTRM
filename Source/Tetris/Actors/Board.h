// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.generated.h"

class ABlock;
class ATetromino;

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

	// Input Interface
	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void RotateCW();
	void RotateCCW();
	void Drop();

private:
	void OnDescendTimer();

	void SetActivePosition(const FIntPoint& newPosition);
	void RepositionActiveTetromino();
	void SpawnNewTetromino();

	// Return true if move is successful
	bool TryMoveTetromino(const FIntPoint& offset);

	void PlaceBlocks(const TArray< FIntPoint >& positions );
	void PlaceBlocks(const FIntPoint& position);
	void SetTileFilled(uint8 row, uint8 col, bool filled);

	FBox2D GetActiveBounds() const;

	static const uint8 s_gridRows = 24;
	static const uint8 s_gridCols = 10;
	struct TileData
	{
		bool filled;
		ABlock* block;
	};
	TileData m_grid[s_gridRows][s_gridCols];

	ATetromino* m_activeTetromino;
	FIntPoint m_activePosition;

	FTimerHandle m_dropTimerHandle;

public:
	UPROPERTY(EditAnywhere, Category = "Tetris Actors")
		TSubclassOf<ATetromino> TetrominoClass;

	UPROPERTY(EditAnywhere, Category = "Tetris Actors")
		TSubclassOf<ABlock> BlockClass;
};
