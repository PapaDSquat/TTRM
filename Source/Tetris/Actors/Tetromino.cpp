// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetromino.h"
#include "Block.h"

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
	
	SpawnBlocks();
	SetType(GetRandomType());
	SetRotation(GetRandomRotation());
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

ETetrominoType ATetromino::GetRandomType()
{
	return (ETetrominoType)FMath::RandRange(0, (int32)ETetrominoType::Count - 1);;
}

uint8 ATetromino::GetRandomRotation()
{
	return FMath::RandRange(0, s_rotations - 1);
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
	for (uint8 i = 0; i < s_totalBlocks; ++i)
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

void ATetromino::SetType(ETetrominoType type)
{
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
	}

	RepositionBlocks();
}