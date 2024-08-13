// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SNS_S_SettingsData.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct  SIMPLENARRATIVESYSTEM_API FSNS_S_SettingsData
{
	GENERATED_BODY()

	/* Should show subtitles? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bSubtitlesEnabled;

	/* Should show who is speaking? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bSpeakerNameEnabled;

	// not working with rich text
	///* Text size */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//float SubtitlesSize;

	/* Alpha of widget background color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SubtitlesBackgroundOpacity;

	/* Color of widget background */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor SubtitlesBackgroundColor;

	/* Color of dialogue text */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor SubtitlesTextColor;

	/* Amount of dialogue and speaker text's outline */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SubtitlesTextOutlineAmount;

};
