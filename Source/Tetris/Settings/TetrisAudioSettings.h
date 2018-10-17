// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TetrisAudioSettings.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TETRIS_API FTetrisAudioSettings
{
	GENERATED_USTRUCT_BODY()
	
	FTetrisAudioSettings()
		: MusicEnabled(true)
		, SoundEffectsEnabled(true)
		, Volume(1.f)
	{
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris|Audio")
	bool MusicEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris|Audio")
	bool SoundEffectsEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris|Audio")
	float Volume;
};
