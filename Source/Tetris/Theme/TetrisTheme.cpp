// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisTheme.h"

static const FTetrisTheme s_default;
const FTetrisTheme& FTetrisTheme::DEFAULT()
{
	return s_default;
}

//==============================================================================

const FTetrisTheme& UTetrisThemeCollection::GetTheme(const FName& themeName)
{
	FTetrisTheme* foundTheme = Themes.FindByPredicate([themeName](const FTetrisTheme& theme)
	{
		return theme.ThemeID == themeName;
	});

	if (foundTheme)
	{
		return *foundTheme;
	}
	return FTetrisTheme::DEFAULT();
}

