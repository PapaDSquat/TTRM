// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TetrisAudioSettings.h"
#include "TetrisSettings.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UTetrisSettings : public USaveGame
{
	GENERATED_BODY()


public:
	UTetrisSettings()
		: SaveSlotName(TEXT("TestSaveSlot"))
		, UserIndex(0)
	{}

	UPROPERTY(VisibleAnywhere, Category = "Save")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Save")
	uint32 UserIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Tetris")
	FTetrisAudioSettings AudioSettings;
};
