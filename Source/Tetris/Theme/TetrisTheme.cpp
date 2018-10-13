// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisTheme.h"

const FTetrisTheme& FTetrisTheme::DEFAULT()
{
	static const FTetrisTheme s_default;
	return s_default;
}

//==============================================================================

const TArray<FTetrisTheme>& UTetrisThemeCollection::DEFAULT()
{
	static const TArray<FTetrisTheme> s_default;
	return s_default;
}

void UTetrisThemeCollection::PostLoad()
{
	Super::PostLoad();

	// Maybe a bad idea...shouldn't happen when we load the asset in Editor
	for (FTetrisTheme& theme : Themes)
	{
		if (theme.BlockTexture == nullptr)
			continue; 

		for (FBlockTheme& blockTheme : theme.BlockColors)
		{
			if (blockTheme.Texture == nullptr)
			{
				blockTheme.Texture = theme.BlockTexture;
			}
		}
	}
}

const FTetrisTheme& UTetrisThemeCollection::GetTheme(const FName& themeName) const
{
	const FTetrisTheme* foundTheme = Themes.FindByPredicate([themeName](const FTetrisTheme& theme)
	{
		return theme.ThemeID == themeName;
	});

	if (foundTheme)
	{
		return *foundTheme;
	}
	return FTetrisTheme::DEFAULT();
}

const TArray<FTetrisTheme>& UTetrisThemeCollection::GetAllThemes() const
{
	return Themes;
}

