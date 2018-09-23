// Fill out your copyright notice in the Description page of Project Settings.

#include "ClassicGameMode.h"



AClassicGameMode::AClassicGameMode()
{

}

void AClassicGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AClassicGameMode::InitGameState()
{
	Super::InitGameState();
	
}

void AClassicGameMode::BeginPlay()
{
	Super::BeginPlay();
}
