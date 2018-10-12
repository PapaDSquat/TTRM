// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundCue.h"
#include "TetrisTheme.generated.h"

USTRUCT(BlueprintType)
struct FBlockTheme
{
	GENERATED_USTRUCT_BODY()

	FBlockTheme()
		: Tint(FColor::White)
		, Opacity(1.f)
	{}


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Texture;

	UPROPERTY(EditAnywhere)
	FColor Tint;

	UPROPERTY(EditAnywhere)
	float Opacity;
};

USTRUCT(BlueprintType)
struct TETRIS_API FTetrisTheme
{
	GENERATED_USTRUCT_BODY()

	static const FTetrisTheme& DEFAULT();

	FTetrisTheme()
		: GhostPieceAlpha( 0.5f )
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FName ThemeID;

	/*
	Graphics
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	UTexture* BlockTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	TArray<FBlockTheme> BlockColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	UTexture* BackgroundTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	float GhostPieceAlpha;

	/*
	Audio
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* Music;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* DropPieceSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* ClearLinesSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* TetrisSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* LevelUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* GameOverSound;
};

/**
* Create UDataAsset of this type in Editor to fill with themes
*/
UCLASS()
class TETRIS_API UTetrisThemeCollection : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void PostLoad() override;

	const FTetrisTheme& GetTheme(const FName& themeName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTetrisTheme> Themes;
};
