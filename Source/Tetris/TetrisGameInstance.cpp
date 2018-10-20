// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisGameInstance.h"



UTetrisGameInstance::UTetrisGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_audioMgr = ObjectInitializer.CreateDefaultSubobject<UTetrisAudioManager>(this, TEXT("TetrisAudioManager"));
}

void UTetrisGameInstance::Init()
{
	Super::Init();
	
	m_audioMgr->Initialize({});
}

void UTetrisGameInstance::Shutdown()
{
	m_audioMgr->Uninitialize();

	Super::Shutdown();
}
