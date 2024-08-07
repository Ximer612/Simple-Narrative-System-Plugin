// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Structs/SNS_S_SettingsData.h"
#include "GameFramework/SaveGame.h"
#include "SNS_SettingsSaveGame.generated.h"

/**
 * 
 */
UCLASS(config = NarrativeSystemSettings)
class SIMPLENARRATIVESYSTEM_API USNS_SettingsSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	/* Should show subtitles? */
	UPROPERTY(GlobalConfig, VisibleAnywhere, BlueprintReadOnly)
	bool bSubtitlesEnabled;

	/* Should show who is speaking? */
	UPROPERTY(GlobalConfig, VisibleAnywhere, BlueprintReadOnly)
	bool bSpeakerNameEnabled;

	/* Text size */
	UPROPERTY(GlobalConfig, VisibleAnywhere, BlueprintReadOnly)
	int32 SubtitlesSize;

	/* Colors, outline, bold, italic */
	UPROPERTY(GlobalConfig, VisibleAnywhere, BlueprintReadOnly)
	int32 SubtitlesType;

	/* Alpha of widget background color */
	UPROPERTY(GlobalConfig, VisibleAnywhere, BlueprintReadOnly)
	int32 SubtitlesBackgroundOpacity;

	/* Color of widget background */
	UPROPERTY(GlobalConfig, VisibleAnywhere, BlueprintReadOnly)
	int32 SubtitlesBackgroundColor;

	/* Color of dialogue text */
	UPROPERTY(GlobalConfig, VisibleAnywhere, BlueprintReadOnly)
	int32 SubtitlesTextColor;

	/* Amount of dialogue and speaker text's outline */
	UPROPERTY(GlobalConfig, VisibleAnywhere, BlueprintReadOnly)
	int32 SubtitlesTextOutlineAmount;
};
