// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisPlayerState.h"


ATetrisPlayerState::ATetrisPlayerState()
{
}

FPlayerRoundStats& ATetrisPlayerState::GetRoundStats()
{
	return m_roundStats;
}
