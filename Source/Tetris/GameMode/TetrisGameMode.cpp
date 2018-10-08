// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisGameMode.h"
#include "../Theme/TetrisTheme.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"


ATetrisGameMode::ATetrisGameMode()
	: Lines(0)
	, Score(0)
	, InitialGameSpeed(1.f)
	, InitialTetrominoDropTime(1.f)
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ATetrisGameMode::BeginPlay()
{
	Super::BeginPlay();

	m_startTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}

void ATetrisGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_currentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}

const FTetrisTheme* ATetrisGameMode::GetCurrentTheme() const
{
	// TODO
	// static const FString Context = FString(TEXT("ATetrisGameMode::GetCurrentTheme"));
	// return ThemesData->FindRow<FTetrisTheme>(ThemeName, Context);
	return nullptr;
}

float ATetrisGameMode::GetGameSpeed() const
{
	return InitialGameSpeed;
}

float ATetrisGameMode::GetTetrominoDropTime() const
{
	return InitialTetrominoDropTime;
}

FString ATetrisGameMode::GetRoundTimeString() const
{
	const FTimespan elapsed = FTimespan::FromSeconds(m_currentTime - m_startTime);
	
	FString hours = elapsed.ToString(TEXT("%h"));
	FString minutes = elapsed.ToString(TEXT("%m"));
	FString seconds = elapsed.ToString(TEXT("%s"));
	hours.RemoveAt(0); // Remove the + and -
	minutes.RemoveAt(0);
	seconds.RemoveAt(0);
	
	const FString outputStr = hours + ":" + minutes + ":" + seconds;
	return outputStr;
}
