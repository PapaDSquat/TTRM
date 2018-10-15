// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisGameInstance.h"



UTetrisGameInstance::UTetrisGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//m_audioMgr = ObjectInitializer.CreateDefaultSubobject<UTetrisAudioManager>(this, TEXT("TetrisAudioManager"));
}

void UTetrisGameInstance::Init()
{
	m_audioMgr = NewObject<UTetrisAudioManager>(UTetrisAudioManager::StaticClass());
}
