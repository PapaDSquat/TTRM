// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetromino.h"
#include "../GameMode/TetrisGameMode.h"

const uint8 ATetromino::s_totalBlocks;
const uint8 ATetromino::s_maxGridSize;
const uint8 ATetromino::s_rotations;

static const float s_blockSize = 100.f;

ATetromino::ATetromino()
{

}

// Called when the game starts or when spawned
void ATetromino::BeginPlay()
{
	Super::BeginPlay();
	
	if (Themes.Num() == 0)
	{
		Themes.Push(FBlockTheme()); // Create default theme
	}

	ATetrisGameMode* gameMode = (ATetrisGameMode*)GetWorld()->GetAuthGameMode();
	

	// Setup themes
	// TODO: Move this out of here, to Board?
	static int8 s_themeCopies = 3;
	m_themeBag.Add(gameMode->GetCurrentTheme().BlockColors, 2);

	SpawnBlocks();
	Randomize();
}

// Called every frame
void ATetromino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATetromino::RotateCW()
{
	m_rotationIndex = (m_rotationIndex + 1) % s_rotations;
	RepositionBlocks();
}

void ATetromino::RotateCCW()
{
	m_rotationIndex = m_rotationIndex == 0 ? s_rotations - 1 : (m_rotationIndex - 1);
	RepositionBlocks();
}

void ATetromino::Randomize()
{
	SetTheme(GetRandomTheme());
	SetType(GetRandomType());
	SetRotation(GetRandomRotation());
}

const TArray< FIntPoint >& ATetromino::GetGridPositions() const
{
	return m_rotations[m_rotationIndex].m_positions;
}

FBox2D ATetromino::GetBounds() const
{
	// TODO: cache
	TArray<FVector2D> points;
	const auto& rotation = m_rotations[m_rotationIndex];
	for (const auto& position : rotation.m_positions)
	{
		points.Push(position);
	}
	return FBox2D(points);
}

FIntPoint ATetromino::GetRealSize() const
{
	FIntPoint size( 0,0 );
	const auto& rotation = m_rotations[m_rotationIndex];
	for (const auto& position : rotation.m_positions)
	{
		//size.X = FMath::Max(size.X, position.X);
		//size.X = FMath::Max(size.X, position.X);
	}
	return size;
}

void ATetromino::SetIsShadow(bool isShadow)
{
	// TODO
	m_isShadow = isShadow;
}

const FBlockTheme& ATetromino::GetTheme() const
{
	return m_theme;
}

void ATetromino::SetTheme(const FBlockTheme& theme)
{
	m_theme = theme;

	if (m_isShadow)
	{
		m_theme.Opacity = 0.50f; // TODO: Parametrize
	}

	for (uint8 i = 0; i < s_totalBlocks; ++i)
	{
		m_blocks[i]->SetTheme(m_theme);
	}
}

ETetrominoType ATetromino::GetRandomType()
{
	return (ETetrominoType)FMath::RandRange(0, (int32)ETetrominoType::Count - 1);
}

uint8 ATetromino::GetRandomRotation()
{
	return FMath::RandRange(0, s_rotations - 1);
}

const FBlockTheme& ATetromino::GetRandomTheme()
{
	return m_themeBag.Pull();
}

void ATetromino::SpawnBlocks()
{
	for (uint8 i = 0; i < s_totalBlocks; ++i)
	{
		FVector location(0.0f, 0.0f, 0.0f);
		FRotator rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters spawnInfo;
		spawnInfo.Owner = this;
		m_blocks[i] = GetWorld()->SpawnActor<ABlock>(BlockClass, location, rotation, spawnInfo);

		FAttachmentTransformRules attachRules(EAttachmentRule::KeepRelative, false);
		m_blocks[i]->AttachToActor(this, attachRules);
	}
}

void ATetromino::RepositionBlocks()
{
	PositionBlocks(m_rotations[m_rotationIndex]);
}

void ATetromino::PositionBlocks(const RotationConfig& config)
{
	for (uint8 i = 0; i < config.m_positions.Num(); ++i)
	{
		const FIntPoint& gridPosition = config.m_positions[i];
		if (ABlock* block = m_blocks[i])
		{
			const FVector location(gridPosition.Y * s_blockSize, 0.f, -gridPosition.X * s_blockSize);
			block->SetActorRelativeLocation(location);
		}
	}
}

void ATetromino::SetRotation(uint8 index)
{
	check(index < s_rotations);

	if (m_rotationIndex != index)
	{
		m_rotationIndex = index;

		RepositionBlocks();
	}
}

void ATetromino::CopyConfigTo(ATetromino* other) const
{
	other->SetTheme(m_theme);
	other->SetType(m_type);
	other->SetRotation(m_rotationIndex);
}

void ATetromino::SwapConfig(ATetromino* other)
{
	const FBlockTheme theme = m_theme;
	const ETetrominoType type = m_type;
	const uint8 rotation = m_rotationIndex;

	SetTheme(other->m_theme);
	SetType(other->m_type);
	SetRotation(other->m_rotationIndex);

	other->SetTheme(theme);
	other->SetType(type);
	other->SetRotation(rotation);
}

ETetrominoType ATetromino::GetType() const
{
	return m_type;
}

void ATetromino::SetType(ETetrominoType type)
{
	m_type = type;

	switch (type)
	{
	case ETetrominoType::I:
		m_rotations[0] = RotationConfig({ FIntPoint(1,0), FIntPoint(1,1), FIntPoint(1,2), FIntPoint(1,3) });
		m_rotations[1] = RotationConfig({ FIntPoint(0,2), FIntPoint(1,2), FIntPoint(2,2), FIntPoint(3,2) });
		m_rotations[2] = RotationConfig({ FIntPoint(2,0), FIntPoint(2,1), FIntPoint(2,2), FIntPoint(2,3) });
		m_rotations[3] = RotationConfig({ FIntPoint(0,1), FIntPoint(1,1), FIntPoint(2,1), FIntPoint(3,1) });
	break;
	case ETetrominoType::O:
		m_rotations[0] = RotationConfig({ FIntPoint(0,1), FIntPoint(0,2), FIntPoint(1,1), FIntPoint(1, 2) });
		m_rotations[1] = RotationConfig({ FIntPoint(0,1), FIntPoint(0,2), FIntPoint(1,1), FIntPoint(1, 2) });
		m_rotations[2] = RotationConfig({ FIntPoint(0,1), FIntPoint(0,2), FIntPoint(1,1), FIntPoint(1, 2) });
		m_rotations[3] = RotationConfig({ FIntPoint(0,1), FIntPoint(0,2), FIntPoint(1,1), FIntPoint(1, 2) });
	break;
	case ETetrominoType::T:
		m_rotations[0] = RotationConfig({ FIntPoint(1,0), FIntPoint(1,1), FIntPoint(1,2), FIntPoint(2,1) });
		m_rotations[1] = RotationConfig({ FIntPoint(0,1), FIntPoint(1,0), FIntPoint(1,1), FIntPoint(2,1) });
		m_rotations[2] = RotationConfig({ FIntPoint(1,1), FIntPoint(2,0), FIntPoint(2,1), FIntPoint(2,2) });
		m_rotations[3] = RotationConfig({ FIntPoint(0,1), FIntPoint(1,1), FIntPoint(1,2), FIntPoint(2,1) });
	break;
	case ETetrominoType::L:
		m_rotations[0] = RotationConfig({ FIntPoint(0,1), FIntPoint(1,1), FIntPoint(2,1), FIntPoint(2,2) });
		m_rotations[1] = RotationConfig({ FIntPoint(1,0), FIntPoint(1,1), FIntPoint(1,2), FIntPoint(2,0) });
		m_rotations[2] = RotationConfig({ FIntPoint(0,0), FIntPoint(0,1), FIntPoint(1,1), FIntPoint(2,1) });
		m_rotations[3] = RotationConfig({ FIntPoint(1,2), FIntPoint(2,0), FIntPoint(2,1), FIntPoint(2,2) });
	break;
	case ETetrominoType::J:
		m_rotations[0] = RotationConfig({ FIntPoint(0,1), FIntPoint(1,1), FIntPoint(2,0), FIntPoint(2,1) });
		m_rotations[1] = RotationConfig({ FIntPoint(1,0), FIntPoint(2,0), FIntPoint(2,1), FIntPoint(2,2) });
		m_rotations[2] = RotationConfig({ FIntPoint(0,1), FIntPoint(0,2), FIntPoint(1,1), FIntPoint(2,1) });
		m_rotations[3] = RotationConfig({ FIntPoint(1,0), FIntPoint(1,1), FIntPoint(1,2), FIntPoint(2,2) });
	break;
	case ETetrominoType::S:
		m_rotations[0] = RotationConfig({ FIntPoint(1,1), FIntPoint(1,2), FIntPoint(2,0), FIntPoint(2,1) });
		m_rotations[1] = RotationConfig({ FIntPoint(0,0), FIntPoint(1,0), FIntPoint(1,1), FIntPoint(2,1) });
		m_rotations[2] = RotationConfig({ FIntPoint(1,1), FIntPoint(1,2), FIntPoint(2,0), FIntPoint(2,1) });
		m_rotations[3] = RotationConfig({ FIntPoint(0,0), FIntPoint(1,0), FIntPoint(1,1), FIntPoint(2,1) });
	break;
	case ETetrominoType::Z:
		m_rotations[0] = RotationConfig({ FIntPoint(1,0), FIntPoint(1,1), FIntPoint(2,1), FIntPoint(2,2) });
		m_rotations[1] = RotationConfig({ FIntPoint(0,2), FIntPoint(1,1), FIntPoint(1,2), FIntPoint(2,1) });
		m_rotations[2] = RotationConfig({ FIntPoint(1,0), FIntPoint(1,1), FIntPoint(2,1), FIntPoint(2,2) });
		m_rotations[3] = RotationConfig({ FIntPoint(0,2), FIntPoint(1,1), FIntPoint(1,2), FIntPoint(2,1) });
	break;
	case ETetrominoType::Count:
		m_rotations[0] = RotationConfig();
		m_rotations[1] = RotationConfig();
		m_rotations[2] = RotationConfig();
		m_rotations[3] = RotationConfig();
		break;
	}

	const bool hideBlocks = (type == ETetrominoType::Count);
	for (uint8 i = 0; i < s_totalBlocks; ++i)
	{
		m_blocks[i]->SetActorHiddenInGame(hideBlocks);
	}

	RepositionBlocks();
}