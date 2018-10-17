// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Settings/TetrisAudioSettings.h"
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

	struct InitializeParams
	{
	};
	void Initialize( const InitializeParams& params );

	// Plays a sound effect once.
	UFUNCTION(BlueprintCallable, Category = "Tetris|Audio")
	void PlaySound(USoundCue* soundCue);

	// Sets the current Music cue. Does not play the Music automatically.
	UFUNCTION(BlueprintCallable, Category = "Tetris|Audio")
	void SetMusic(USoundCue* musicCue);

	// Plays the current Music cue, if any.
	UFUNCTION(BlueprintCallable, Category = "Tetris|Audio")
	void PlayMusic();

	// Stops the current Music cue, if any.
	UFUNCTION(BlueprintCallable, Category = "Tetris|Audio")
	void StopMusic();

	UFUNCTION(BlueprintCallable, Category = "Tetris|Audio")
	bool IsMusicSet();

	UFUNCTION(BlueprintCallable, Category = "Tetris|Audio")
	bool IsMusicPlaying();

	UFUNCTION(BlueprintCallable, Category = "Tetris|Audio")
	FTetrisAudioSettings& GetAudioSettings();

	UFUNCTION(BlueprintCallable, Category = "Tetris|Audio")
	bool SaveSettings(const FTetrisAudioSettings& newSettings);

private:
	bool LoadSettings();

	// Event Callbacks
	UFUNCTION()
	void OnMusicFinished();

	UWorld* m_world;

	USoundCue* m_musicCue;
	UAudioComponent* m_musicAudioComponent;

	FTetrisAudioSettings m_audioSettings;
};
