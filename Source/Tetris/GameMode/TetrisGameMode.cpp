// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisGameMode.h"

#include "Kismet/GameplayStatics.h"


ATetrisGameMode::ATetrisGameMode()
	: Lines(0)
	, Score(0)
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
