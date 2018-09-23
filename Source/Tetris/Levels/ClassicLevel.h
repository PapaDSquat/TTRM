// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "ClassicLevel.generated.h"

class ATetromino;
class ABoard;

/**
 * 
 */
UCLASS()
class TETRIS_API AClassicLevel : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	AClassicLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
