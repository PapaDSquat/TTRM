// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisGameMode.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "../Theme/TetrisTheme.h"
#include "../Audio/TetrisAudioManager.h"
#include "../TetrisGameInstance.h"

ATetrisGameMode::ATetrisGameMode()
	: Lines(0)
	, Score(0)
	, InitialGameSpeed(1.f)
	, InitialTetrominoDropTime(1.f)
	, m_gameStarted( false )
	, m_gamePaused( false)
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ATetrisGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Force Themes to load
	//Themes.LoadSynchronous();
}

void ATetrisGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_currentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}

UTetrisGameInstance* ATetrisGameMode::GetTetrisGameInstance()
{
	return Cast<UTetrisGameInstance>(GetGameInstance());
}

void ATetrisGameMode::FireGameEvent(EGameEventType eventType)
{
	// GameEvent is always fired first
	OnGameEvent().Broadcast(eventType);
	switch (eventType)
	{
	case EGameEventType::Start: OnGameStart.Broadcast(); break;
	case EGameEventType::Restart: OnGameRestart.Broadcast(); break;
	case EGameEventType::End: OnGameEnd.Broadcast(); break;
	case EGameEventType::Pause: OnGamePause.Broadcast(); break;
	case EGameEventType::Unpause: OnGameUnPause.Broadcast(); break;
	}
}

const FTetrisTheme& ATetrisGameMode::GetCurrentTheme() const
{
	return m_currentTheme;
}

const TArray<FTetrisTheme>& ATetrisGameMode::GetAllThemes() const
{
	if (Themes)
	{
		return Themes->GetAllThemes();
	}
	return UTetrisThemeCollection::DEFAULT();
}

void ATetrisGameMode::StartGame()
{
	if (!m_gameStarted)
	{
		m_gameStarted = true;

		m_startTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

		if (Themes && 
			m_currentTheme == FTetrisTheme::DEFAULT())
		{
			SetTheme(Themes->GetTheme(DefaultThemeID), false);
		}

		const auto audioMgr = GetTetrisGameInstance()->GetAudioManager();
		audioMgr->StopMusic();
		audioMgr->PlayMusic();

		FireGameEvent(EGameEventType::Start);
	}
}

void ATetrisGameMode::EndGame()
{
	if (m_gameStarted)
	{
		m_gameStarted = false;
		FireGameEvent(EGameEventType::End);
	}
}

void ATetrisGameMode::RestartGame()
{
	EndGame();
	StartGame();
	FireGameEvent(EGameEventType::Restart);
}

void ATetrisGameMode::PauseGame()
{
	if (!m_gamePaused)
	{
		m_gamePaused = true;
		FireGameEvent(EGameEventType::Pause);
	}
}

void ATetrisGameMode::UnpauseGame()
{
	if (m_gamePaused)
	{
		m_gamePaused = false;
		FireGameEvent(EGameEventType::Unpause);
	}
}

bool ATetrisGameMode::IsGameStarted() const
{
	return m_gameStarted;
}

bool ATetrisGameMode::IsGamePaused() const
{
	return m_gamePaused;
}

bool ATetrisGameMode::SetTheme(const FName& themeID)
{
	if (Themes)
	{
		return SetTheme(Themes->GetTheme(themeID));
	}
	return false;
}

bool ATetrisGameMode::SetTheme(const FTetrisTheme& theme, bool restart /*= true*/)
{
	if (m_currentTheme.ThemeID != theme.ThemeID)
	{
		m_currentTheme = theme;
		GetTetrisGameInstance()->GetAudioManager()->SetMusic( m_currentTheme.Music );
		OnThemeChange.Broadcast(m_currentTheme);
		if(restart)
			RestartGame();
		return true;
	}
	return false;
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
