// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundCue.h"
#include "TetrisTheme.generated.h"



USTRUCT(BlueprintType)
struct FThemeMaterialParams
{
	GENERATED_USTRUCT_BODY()

		FThemeMaterialParams()
		: Tint(FColor::White)
		, Opacity(1.f)
	{}

	// Overrides FTetrisTheme::BlockTexture, if specified
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Tint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Opacity;
};

USTRUCT(BlueprintType)
struct FBlockTheme
{
	GENERATED_USTRUCT_BODY()

	FBlockTheme()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FThemeMaterialParams MaterialParams;
};

USTRUCT(BlueprintType)
struct TETRIS_API FTetrisTheme
{
	GENERATED_USTRUCT_BODY()

	static const FTetrisTheme& DEFAULT();

	FTetrisTheme()
	{}

	bool operator == (const FTetrisTheme& rhs) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FName ThemeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FName ThemeName;

	/*
	** GRAPHICS
	*/

	/*
	**	Uses the same block texture for every Tetromino type. Can be overriden individually in BlockThemes.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	UTexture* SingleBlockTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	TArray<FBlockTheme> BlockThemes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	FThemeMaterialParams BackgroundTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	FThemeMaterialParams BorderTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	FThemeMaterialParams BackTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	FThemeMaterialParams GhostPieceTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FThemeMaterialParams GameOverPieceTheme;

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

	static const TArray<FTetrisTheme>& DEFAULT();
	const FTetrisTheme& GetTheme(const FName& themeName) const;
	const TArray<FTetrisTheme>& GetAllThemes() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTetrisTheme> Themes;
};
