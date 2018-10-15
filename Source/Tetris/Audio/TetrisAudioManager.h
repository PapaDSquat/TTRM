// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TetrisAudioManager.generated.h"

class USoundCue;

/**
 * Tetris Audio Manger
 ***************************
 * Play Music and Sounds
 */
UCLASS(BlueprintType)
class TETRIS_API UTetrisAudioManager : public UObject
{
	GENERATED_BODY()
	
public:
	UTetrisAudioManager();

	void PlaySound(USoundCue* soundCue);

	void PlayMusic(USoundCue* musicCue);
	void StopMusic();
	bool IsMusicPlaying();

private:
	USoundCue* m_musicCue;
	UAudioComponent* m_music;
};
