// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
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
	TSoftObjectPtr<UTexture> Texture;

	UPROPERTY(EditAnywhere)
	FColor Tint;

	UPROPERTY(EditAnywhere)
	float Opacity;
};

USTRUCT(BlueprintType)
struct TETRIS_API FTetrisTheme : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FTetrisTheme()
	{}

	/*
		Graphics
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	TSoftObjectPtr<UTexture> BlockTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	TArray<FBlockTheme> BlockColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	TSoftObjectPtr<UTexture> BackgroundTexture;

	/*
		Audio
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundCue> Music;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundCue> DropPieceSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundCue> ClearLinesSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundCue> TetrisSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundCue> DeathSound;
};

