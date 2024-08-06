// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SNS_S_SettingsData.generated.h"

/**
 * 
 */
USTRUCT()
struct  SIMPLENARRATIVESYSTEM_API FSNS_S_SettingsData
{
	GENERATED_BODY()

	/* Should show subtitles? */
	UPROPERTY()
	bool bSubtitlesEnabled;

	/* Should show who is speaking? */
	UPROPERTY()
	bool bSpeakerNameEnabled;

	/* Text size */
	UPROPERTY()
	int32 SubtitlesSize;

	/* Colors, outline, bold, italic */
	UPROPERTY()
	int32 SubtitlesType;

	/* Alpha of widget background color */
	UPROPERTY()
	int32 SubtitlesBackgroundOpacity;

	/* Color of widget background */
	UPROPERTY()
	int32 SubtitlesBackgroundColor;

	/* Color of dialogue text */
	UPROPERTY()
	int32 SubtitlesTextColor;

	/* Amount of dialogue and speaker text's outline */
	UPROPERTY()
	int32 SubtitlesTextOutlineAmount;
	
};
