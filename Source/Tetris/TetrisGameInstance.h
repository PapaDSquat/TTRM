// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Audio/TetrisAudioManager.h"
#include "TetrisGameInstance.generated.h"

/**
 * Tetris Game Instance
 ***************************
 * Owns Audio Manager
 */

UCLASS()
class TETRIS_API UTetrisGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTetrisGameInstance(const FObjectInitializer& ObjectInitializer);

	// UGameInstance Interface
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	UTetrisAudioManager* GetAudioManager() { return m_audioMgr; }

private:
	UPROPERTY()
	UTetrisAudioManager* m_audioMgr;
};
