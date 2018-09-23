// Fill out your copyright notice in the Description page of Project Settings.

#include "Board.h"

#include "../Actors/Block.h"
#include "../Actors/Tetromino.h"


const uint8 ABoard::s_gridRows;
const uint8 ABoard::s_gridCols;

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
	// Init board
	for (uint8 r = 0; r < s_gridRows; ++r)
	{
		for (uint8 c = 0; c < s_gridCols; ++c)
		{
			// Move and store
			FVector worldLocation(0.f, 0.f, 0.f);
			FRotator rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters spawnInfo;
			spawnInfo.Owner = this;
			ABlock* block = GetWorld()->SpawnActor<ABlock>(BlockClass, worldLocation, rotation, spawnInfo);

			FAttachmentTransformRules attachRules(EAttachmentRule::KeepRelative, false);
			block->AttachToActor(this, attachRules);

			const FVector location(c * 100.f, 0.f, -r * 100.f);
			block->SetActorRelativeLocation(location);

			block->SetActorHiddenInGame(true);

			m_grid[r][c] = { 0, block };
		}
	}

	{
		// Spawn Active Tetromino
		FVector location(0.0f, 0.0f, 0.0f);
		FRotator rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters spawnInfo;
		spawnInfo.Owner = this;
		m_activeTetromino = GetWorld()->SpawnActor< ATetromino >(TetrominoClass, location, rotation, spawnInfo);

		FAttachmentTransformRules attachRules(EAttachmentRule::KeepRelative, false);
		m_activeTetromino->AttachToActor(this, attachRules);
	}


	m_activePosition.X = 0;
	m_activePosition.Y = s_gridCols / 2;
	RepositionActiveTetromino();

	GetWorldTimerManager().SetTimer(m_dropTimerHandle, this, &ABoard::OnDescendTimer, 1.25f, true, 1.0f);
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::MoveLeft()
{
	const auto offset = FIntPoint(0, -1);
	if (TryMoveTetromino(offset))
	{
		SetActivePosition(m_activePosition + offset);
	}
}

void ABoard::MoveRight()
{
	const auto offset = FIntPoint(0, 1);
	if (TryMoveTetromino(offset))
	{
		SetActivePosition(m_activePosition + offset);
	}
}

void ABoard::MoveDown()
{
	const auto offset = FIntPoint(1, 0);
	if (TryMoveTetromino(offset))
	{
		SetActivePosition(m_activePosition + offset);
	}
	else
	{
		PlaceBlocks(m_activePosition);
		SpawnNewTetromino();
	}
}

void ABoard::RotateCW()
{
	m_activeTetromino->RotateCW();
}

void ABoard::RotateCCW()
{
	m_activeTetromino->RotateCCW();
}

void ABoard::Drop()
{
	auto offset = FIntPoint(1, 0);
	while (TryMoveTetromino(offset))
	{
		offset.X += 1;
	}

	offset.X -= 1;
	PlaceBlocks(m_activePosition + offset);
	SpawnNewTetromino();
}

void ABoard::OnDescendTimer()
{
	MoveDown();
	// TODO: Reset timer
}

void ABoard::SetActivePosition(const FIntPoint& newPosition)
{
	if (m_activePosition != newPosition)
	{
		m_activePosition = newPosition;
		RepositionActiveTetromino();
		return;
	}
}

bool ABoard::TryMoveTetromino(const FIntPoint& offset)
{
	TArray< FIntPoint > localGridPositions = m_activeTetromino->GetGridPositions();
	TArray< FIntPoint > newGridPositions;

	for (auto& localPos : localGridPositions)
	{
		const FIntPoint newPos = localPos + m_activePosition + offset;
		newGridPositions.Push(newPos);
	}

	for (auto& pos : newGridPositions)
	{
		if (pos.X >= s_gridRows || pos.Y < 0 || pos.Y >= s_gridCols)
			return false;

		if (m_grid[pos.X][pos.Y].filled == 1)
		{
			return false;
		}
	}
	return true;
}



void ABoard::RepositionActiveTetromino()
{
	const FVector location(m_activePosition.Y * 100.f, 0.f, -m_activePosition.X * 100.f);
	m_activeTetromino->SetActorRelativeLocation(location);
}

void ABoard::SpawnNewTetromino()
{
	m_activeTetromino->Randomize();
	m_activePosition.X = m_activePosition.Y = 0;
	RepositionActiveTetromino();
}

void ABoard::PlaceBlocks(const TArray< FIntPoint >& positions)
{
	int32 minRow = 0;
	int32 maxRow = 0;
	for (auto& pos : positions)
	{
		TileData& tileData = m_grid[pos.X][pos.Y];

		tileData.filled = 1;
		tileData.block->SetActorHiddenInGame(false);
		//tileData.block->

		minRow = FMath::Min(minRow, pos.Y);
		maxRow = FMath::Max(maxRow, pos.Y);
	}

	// TODO Try to clear any rows affected
	
}

void ABoard::PlaceBlocks(const FIntPoint& position)
{
	TArray< FIntPoint > localGridPositions = m_activeTetromino->GetGridPositions();
	for (auto& localPos : localGridPositions)
	{
		localPos += position;
	}
	PlaceBlocks(localGridPositions);
}

FBox2D ABoard::GetActiveBounds() const
{
	const auto activePos = FVector2D(m_activePosition.X, m_activePosition.Y);
	auto bounds = m_activeTetromino->GetBounds();
	
	bounds += FBox2D(activePos, activePos);
	return bounds;
}

