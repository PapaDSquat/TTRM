// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisAudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

UTetrisAudioManager::UTetrisAudioManager()
	: m_music(nullptr)
	, m_musicCue(nullptr)
{

}

void UTetrisAudioManager::PlaySound(USoundCue* soundCue)
{
	UGameplayStatics::PlaySound2D(GetWorld(), soundCue);
}

void UTetrisAudioManager::PlayMusic(USoundCue* musicCue)
{
	if (musicCue == nullptr)
		return;

	if (m_music != nullptr
		&& m_musicCue == musicCue)
	{
		if (!IsMusicPlaying())
			m_music->Play();
	}
	else
	{
		m_musicCue = musicCue;

		StopMusic();
		UWorld* world = GetWorld();

		/*m_music = */UGameplayStatics::PlaySound2D(GetWorld(), m_musicCue);
	}
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
