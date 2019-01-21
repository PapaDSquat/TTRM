// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisGameMode.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "../Theme/TetrisTheme.h"
#include "../Audio/TetrisAudioManager.h"
#include "../Player/TetrisPlayerState.h"
#include "../Player/PlayerPawn.h"
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

	PlayerStateClass = ATetrisPlayerState::StaticClass();

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
	case EGameEventType::GameOver: OnGameOver.Broadcast(); break;
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

		m_players.Empty();
		for (auto it = GetWorld()->GetPawnIterator(); it; ++it)
		{
			if (APlayerPawn* playerPawn = Cast< APlayerPawn >(it->Get()))
			{
				m_players.Push(playerPawn);

				playerPawn->GetRoundStats().Reset();
			}
		}

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
	OnStartGameInternal();
}

void ATetrisGameMode::EndGame()
{
	if (m_gameStarted)
	{
		m_gameStarted = false;
		GetTetrisGameInstance()->GetAudioManager()->StopMusic();
		FireGameEvent(EGameEventType::End);
		OnEndGameInternal();

		m_players.Empty();
	}
}

void ATetrisGameMode::GameOver()
{
	EndGame();
	FireGameEvent(EGameEventType::GameOver);
	OnGameOverInternal();
}

void ATetrisGameMode::RestartGame()
{
	EndGame();
	StartGame();
	FireGameEvent(EGameEventType::Restart);
	OnRestartGameInternal();
}

void ATetrisGameMode::PauseGame()
{
	if (!m_gamePaused)
	{
		m_gamePaused = true;
		FireGameEvent(EGameEventType::Pause);
		OnPauseGameInternal();
	}

}

void ATetrisGameMode::UnpauseGame()
{
	if (m_gamePaused)
	{
		m_gamePaused = false;
		FireGameEvent(EGameEventType::Unpause);
		OnUnpauseGameInternal();
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

void ATetrisGameMode::SetNumPlayers(int32 num)
{
	const int32 currentNum = GetNumPlayers();
	const int32 newNum = FMath::Max(1, num); // Minimum 1 player
	if (currentNum == newNum)
		return;

	// Spawn new players
	if (newNum > currentNum)
	{
		for (int32 i = currentNum; i < newNum; ++i)
		{
			UGameplayStatics::CreatePlayer(GetWorld(), i);
		}
	}
	// Remove old players
	else
	{
		for (int32 i = currentNum; i > newNum; --i)
		{
			UGameplayStatics::RemovePlayer(UGameplayStatics::GetPlayerController(GetWorld(), i-1), true);
		}
	}

	RestartGame();
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

float ATetrisGameMode::GetGameSpeed(APlayerPawn* playerPawn) const
{
	return InitialGameSpeed;
}

float ATetrisGameMode::GetTetrominoDropTime(APlayerPawn* playerPawn) const
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
	
	FString outputStr;
	if (hours == "0" || hours == "00")
		outputStr = minutes + ":" + seconds;
	else
		outputStr = hours + ":" + minutes + ":" + seconds;
	return outputStr;
}
