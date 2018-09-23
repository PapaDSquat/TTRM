// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClassicGameMode.generated.h"



/**
 * 
 */
UCLASS()
class TETRIS_API AClassicGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AClassicGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
};
