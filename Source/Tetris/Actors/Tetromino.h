// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.h"
#include "Tetromino.generated.h"

class ABlock;

UENUM()
enum class ETetrominoType
{
	I,
	O,
	T,
	L,
	J,
	S,
	Z,
	Count
};

UCLASS()
class TETRIS_API ATetromino : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetromino();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Input Interface
	void RotateCW();
	void RotateCCW();

	void Randomize();
	void CopyConfig(ATetromino* other) const;

	const TArray< FIntPoint >& GetGridPositions() const;
	FBox2D GetBounds() const;
	FIntPoint GetRealSize() const;

	void SetType(ETetrominoType type);
	void SetIsShadow(bool isShadow);

	const FBlockTheme& GetTheme() const;
	void SetTheme(const FBlockTheme& theme);

	static ETetrominoType GetRandomType();
	static uint8 GetRandomRotation();
	FBlockTheme GetRandomTheme();

private:
	struct RotationConfig
	{
		RotationConfig() {}
		RotationConfig(const TArray< FIntPoint >& positions)
			: m_positions(positions) {}
		TArray< FIntPoint > m_positions; // TODO: change to static array size 4
	};

	void SpawnBlocks();

	void RepositionBlocks();
	void PositionBlocks(const RotationConfig& config);

	void SetRotation(uint8 index);

	ETetrominoType m_type{ ETetrominoType::Count };

	static const uint8 s_maxGridSize = 4;
	uint8 m_grid[s_maxGridSize][s_maxGridSize];

	static const uint8 s_rotations = 4;
	RotationConfig m_rotations[s_rotations];

	uint8 m_rotationIndex{ 0 };

	static const uint8 s_totalBlocks = 4;
	ABlock* m_blocks[s_totalBlocks];

	FBlockTheme m_theme;
	
	bool m_isShadow{ false };

public:
	UPROPERTY(EditAnywhere, Category = "Block")
	TSubclassOf<ABlock> BlockClass;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Block")
	TArray< FBlockTheme > Themes;

	//UPROPERTY(EditAnywhere, Category = "Block")
	//TArray< UMaterial* > AllMaterials;
	//
	//UPROPERTY(EditAnywhere, Category = "Block")
	//UMaterial* ShadowMaterial;
};
