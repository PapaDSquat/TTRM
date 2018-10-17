// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisAudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "../Settings/TetrisAudioSettings.h"
#include "../Settings/TetrisSettings.h"

UTetrisAudioManager::UTetrisAudioManager()
	: m_music(nullptr)
	, m_musicCue(nullptr)
{
}

void UTetrisAudioManager::Initialize(const InitializeParams& params)
{
	m_world = params.World;

	LoadSettings();
}

void UTetrisAudioManager::PlaySound(USoundCue* soundCue)
{
	if (!m_audioSettings.SoundEffectsEnabled)
		return;

	UGameplayStatics::PlaySound2D(m_world, soundCue);
}

void UTetrisAudioManager::SetMusic(USoundCue* musicCue)
{
	StopMusic();
	m_musicCue = musicCue;
	
	if (m_music)
	{
		m_music->OnAudioFinished.RemoveDynamic(this, &UTetrisAudioManager::OnMusicFinished);
	}
	
	m_music = UGameplayStatics::CreateSound2D(m_world, m_musicCue, m_audioSettings.Volume);
	m_music->OnAudioFinished.AddDynamic(this, &UTetrisAudioManager::OnMusicFinished);
}

void UTetrisAudioManager::PlayMusic()
{
	if (!m_audioSettings.MusicEnabled
		|| m_musicCue == nullptr
		|| m_music == nullptr
		|| IsMusicPlaying())
		return;

	m_music->Play();
}

void UTetrisAudioManager::StopMusic()
{
	if (IsMusicPlaying())
	{
		m_music->Stop();
	}
}

bool UTetrisAudioManager::IsMusicPlaying()
{
	return m_music != nullptr && m_musicCue != nullptr && m_music->IsPlaying();
}

void UTetrisAudioManager::OnMusicFinished()
{
	// Loop music
	PlayMusic();
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
