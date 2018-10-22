// Fill out your copyright notice in the Description page of Project Settings.

#include "Board.h"

#include "../Actors/Block.h"
#include "../Actors/Tetromino.h"
#include "../GameMode/TetrisGameMode.h"


const uint8 ABoard::s_gridRows;
const uint8 ABoard::s_gridCols;

// Sets default values
ABoard::ABoard()
: m_canHold( true )
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
	m_gameMode = (ATetrisGameMode*)GetWorld()->GetAuthGameMode();

	for (int8 i = 0; i < (int8)ETetrominoType::Count; ++i)
	{
		m_typeBag.Add((ETetrominoType)i, 2);
	}

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
		// Spawn Tetrominos
		FVector location(0.0f, 0.0f, 0.0f);
		FRotator rotation(0.0f, 0.0f, 0.0f);
		FAttachmentTransformRules attachRules(EAttachmentRule::KeepRelative, false);
		FActorSpawnParameters spawnInfo;
		spawnInfo.Owner = this;

		{
			m_activeTetromino = GetWorld()->SpawnActor< ATetromino >(TetrominoClass, location, rotation, spawnInfo);
			m_activeTetromino->AttachToActor(this, attachRules);
			m_activeTetromino->Initialize({});
		}

		{
			m_nextTetromino = GetWorld()->SpawnActor< ATetromino >(TetrominoClass, location, rotation, spawnInfo);
			m_nextTetromino->AttachToActor(this, attachRules);

			const FVector localLocation(-1475.f, 0.f, -200.f);
			m_nextTetromino->SetActorRelativeLocation(localLocation);
			m_nextTetromino->Initialize({});
		}

		{
			m_holdTetromino = GetWorld()->SpawnActor< ATetromino >(TetrominoClass, location, rotation, spawnInfo);
			m_holdTetromino->AttachToActor(this, attachRules);

			const FVector localLocation(-1475.f, 0.f, -900.f);
			m_holdTetromino->SetActorRelativeLocation(localLocation);
			m_holdTetromino->Initialize({});
		}

		{
			m_ghostTetromino = GetWorld()->SpawnActor< ATetromino >(TetrominoClass, location, rotation, spawnInfo);
			m_ghostTetromino->AttachToActor(this, attachRules);
			m_ghostTetromino->Initialize(ATetromino::InitializeParams({ true }));
		}
	}
	
	ResetBoard();
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::ResetBoard()
{
	for (uint8 r = 0; r < s_gridRows; ++r)
	{
		for (uint8 c = 0; c < s_gridCols; ++c)
		{
			TileData& tile = m_grid[r][c];
			tile.filled = false;
			tile.block->SetActorHiddenInGame(true);
		}
	}
	
	m_typeBag.Refill();

	static int8 s_themeCopies = 3;
	m_themeBag.Empty();
	m_themeBag.Add(m_gameMode->GetCurrentTheme().BlockThemes, 2);

	m_nextTetromino->Randomize();
	m_nextTetromino->SetTheme(GetRandomBlockTheme());
	m_nextTetromino->SetType(GetRandomTetrominoType());

	m_holdTetromino->SetType(ETetrominoType::Count);

	m_activePosition.X = 0;
	m_activePosition.Y = s_gridCols / 2;

	SpawnNewTetromino();
	RepositionActiveTetromino();
	ResetDropTimer();
}

void ABoard::SetPaused(bool paused)
{
	if (paused)
	{
		GetWorldTimerManager().PauseTimer(m_dropTimerHandle);
	}
	else
	{
		GetWorldTimerManager().UnPauseTimer(m_dropTimerHandle);
	}
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

	if (!TryMoveTetromino(FIntPoint::ZeroValue))
	{
		m_activeTetromino->RotateCCW();
	}

	UpdateGhost();
}

void ABoard::RotateCCW()
{
	m_activeTetromino->RotateCCW();

	if (!TryMoveTetromino(FIntPoint::ZeroValue))
	{
		m_activeTetromino->RotateCW();
	}

	UpdateGhost();
}

void ABoard::Drop()
{
	PlaceBlocks(m_ghostPosition);
	SpawnNewTetromino();
}

void ABoard::Hold()
{
	if (!m_canHold)
		return;

	if( m_holdTetromino->GetType() == ETetrominoType::Count )
	{
		m_activeTetromino->CopyConfigTo(m_holdTetromino);
		SpawnNewTetromino();
	}
	else
	{
		m_activeTetromino->SwapConfig(m_holdTetromino);
	}

	// Need to place a piece in order to re-active hold
	m_canHold = false;

	UpdateGhost();
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
	UpdateGhost();
}

void ABoard::SpawnNewTetromino()
{
	{
		m_nextTetromino->CopyConfigTo(m_activeTetromino);
		m_activePosition.X = 0;
		m_activePosition.Y = (s_gridCols / 2) - 1;

		RepositionActiveTetromino();
	}

	{
		m_nextTetromino->Randomize();
		m_nextTetromino->SetTheme(GetRandomBlockTheme());
		m_nextTetromino->SetType(GetRandomTetrominoType());
	}

	m_canHold = true;
}

void ABoard::PlaceBlocks(const TArray< FIntPoint >& positions)
{
	int32 minRow = s_gridRows - 1;
	int32 maxRow = 0;
	for (auto& pos : positions)
	{
		if (pos.X < 0 || pos.Y < 0)
			continue;

		TileData& tileData = m_grid[pos.X][pos.Y];

		SetTileFilled(pos.X, pos.Y, true);

		minRow = FMath::Min(minRow, pos.X);
		maxRow = FMath::Max(maxRow, pos.X);
	}

	if (minRow <= 0)
	{
		m_gameMode->GameOver();
		return;
	}

	// Try to clear any rows affected
	// For now parse from top down, moving one row down at a time
	// TODO: Optimize

	int8 numLines = 0;
	for (int32 row = minRow; row <= maxRow; ++row)
	{
		bool rowFilled = true;
		for (int32 col = 0; col < s_gridCols; ++col)
		{
			if (!m_grid[row][col].filled)
			{
				rowFilled = false;
				break;
			}
		}

		if (rowFilled)
		{
			// Copy data from above line
			for (int32 col = 0; col < s_gridCols; ++col)
			{
				for (int32 boardRow = row; boardRow >= 0; --boardRow)
				{
					// TODO: Copy tile data instead

					TileData& currentTile = m_grid[boardRow][col];
					if (boardRow == 0)
					{
						SetTileFilled(boardRow, col, false);
						continue;
					}

					const TileData& aboveTile = m_grid[boardRow - 1][col];
					CopyTile(aboveTile, currentTile);
				}
			}
			++numLines;
		}
	}

	OnPlaceTetromino().Broadcast();

	if (numLines > 0)
	{
		if (numLines == 4)
			OnClearTetris().Broadcast();
		else
			OnClearLines123().Broadcast(numLines);

		// TODO: Game Mode should listen to event
		m_gameMode->OnClearLines(numLines);
		ResetDropTimer();
	}
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

void ABoard::CopyTile(const TileData& source, TileData& dest) const
{
	dest.filled = source.filled;
	dest.block->SetActorHiddenInGame(!source.filled);
	dest.block->SetTheme(source.block->GetTheme());
}

void ABoard::SetTileFilled(uint8 row, uint8 col, bool filled)
{
	TileData& currentTile = m_grid[row][col];
	currentTile.filled = filled;
	currentTile.block->SetActorHiddenInGame(!filled);
	currentTile.block->SetTheme(m_activeTetromino->GetTheme());
}

FBox2D ABoard::GetActiveBounds() const
{
	const auto activePos = FVector2D(m_activePosition.X, m_activePosition.Y);
	auto bounds = m_activeTetromino->GetBounds();
	
	bounds += FBox2D(activePos, activePos);
	return bounds;
}

void ABoard::UpdateGhost()
{
	// Find ghost position. Start at current position
	FIntPoint offset = FIntPoint::ZeroValue;

	while(TryMoveTetromino(offset))
	{
		++offset.X; // Check one row down
	}
	--offset.X; // Move back up from where it hit

	m_ghostPosition = m_activePosition + offset;
	if (m_ghostPosition == m_activePosition)
	{
		// We overlap ghost ghostPosition, so simply hide it.
		// TODO: Hide any overlapping block
		m_ghostTetromino->SetActorHiddenInGame(m_ghostPosition == m_activePosition);
	}
	else
	{
		m_ghostTetromino->SetActorHiddenInGame(false);
		
		const FVector location(m_ghostPosition.Y * 100.f, 0.f, -m_ghostPosition.X * 100.f);
		m_ghostTetromino->SetActorRelativeLocation(location);
		m_activeTetromino->CopyConfigTo(m_ghostTetromino);
	}
}

void ABoard::ResetDropTimer()
{
	auto& timeMgr = GetWorldTimerManager();
	const bool wasPaused = timeMgr.IsTimerPaused(m_dropTimerHandle);
	timeMgr.ClearTimer(m_dropTimerHandle);
	timeMgr.SetTimer(m_dropTimerHandle, this, &ABoard::OnDescendTimer, m_gameMode->GetTetrominoDropTime(), true, 0.0f);
	if (wasPaused)
		timeMgr.PauseTimer(m_dropTimerHandle);
}

ETetrominoType ABoard::GetRandomTetrominoType()
{
	return m_typeBag.Pull();
}

const FBlockTheme& ABoard::GetRandomBlockTheme()
{
	return m_themeBag.Pull();
}
