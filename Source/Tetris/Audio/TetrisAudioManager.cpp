// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisAudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "../Settings/TetrisAudioSettings.h"
#include "../Settings/TetrisSettings.h"

UTetrisAudioManager::UTetrisAudioManager()
	: m_musicAudioComponent(nullptr)
	, m_musicCue(nullptr)
	, m_world(nullptr)
{
}

void UTetrisAudioManager::Initialize(const InitializeParams& params)
{
	LoadSettings();
}

void UTetrisAudioManager::PlaySound(USoundCue* soundCue)
{
	if (!m_audioSettings.SoundEffectsEnabled)
		return;

	UGameplayStatics::PlaySound2D(GetOuter()->GetWorld(), soundCue);
}

void UTetrisAudioManager::SetMusic(USoundCue* musicCue)
{
	if (m_musicCue == musicCue)
		return;

	StopMusic();
	
	m_musicCue = musicCue;
	
	if (m_musicAudioComponent == nullptr)
	{
		m_musicAudioComponent = UGameplayStatics::CreateSound2D(GetOuter()->GetWorld(), m_musicCue, m_audioSettings.Volume);
		if (m_musicAudioComponent != nullptr)
		{
			m_musicAudioComponent->OnAudioFinished.AddDynamic(this, &UTetrisAudioManager::OnMusicFinished);
		}
	}
	else
	{
		m_musicAudioComponent->SetSound(m_musicCue);
	}
}

void UTetrisAudioManager::PlayMusic()
{
	if (!m_audioSettings.MusicEnabled
		|| !IsMusicSet()
		|| IsMusicPlaying())
		return;

	m_musicAudioComponent->Play();
}

void UTetrisAudioManager::StopMusic()
{
	if (IsMusicPlaying())
	{
		m_musicAudioComponent->Stop();
	}
}

bool UTetrisAudioManager::IsMusicSet()
{
	return m_musicAudioComponent != nullptr && m_musicCue != nullptr;
}

bool UTetrisAudioManager::IsMusicPlaying()
{
	return IsMusicSet() && m_musicAudioComponent->IsPlaying();
}

void UTetrisAudioManager::OnMusicFinished()
{
	PlayMusic(); // Loop music
}

bool UTetrisAudioManager::LoadSettings()
{
	UTetrisSettings* settings = Cast<UTetrisSettings>(UGameplayStatics::CreateSaveGameObject(UTetrisSettings::StaticClass()));
	settings = Cast<UTetrisSettings>(UGameplayStatics::LoadGameFromSlot(settings->SaveSlotName, settings->UserIndex));
	if (settings != nullptr)
	{
		m_audioSettings = settings->AudioSettings;
		return true;
	}
	return false;
}

bool UTetrisAudioManager::SaveSettings(const FTetrisAudioSettings& newSettings)
{
	m_audioSettings = newSettings;

	const bool isMusicPlaying = IsMusicPlaying();
	if (m_audioSettings.MusicEnabled && !isMusicPlaying)
	{
		PlayMusic();
	}
	else if (!m_audioSettings.MusicEnabled && isMusicPlaying)
	{
		StopMusic();
	}

	UTetrisSettings* settings = Cast<UTetrisSettings>(UGameplayStatics::CreateSaveGameObject(UTetrisSettings::StaticClass()));
	settings->AudioSettings = newSettings;
	return UGameplayStatics::SaveGameToSlot(settings, settings->SaveSlotName, settings->UserIndex);
}

FTetrisAudioSettings& UTetrisAudioManager::GetAudioSettings()
{
	return m_audioSettings;
}
