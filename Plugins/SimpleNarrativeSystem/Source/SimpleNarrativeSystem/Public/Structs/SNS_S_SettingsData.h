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

	/* Text size */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SubtitlesSize;

	/* Colors, outline, bold, italic */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SubtitlesType;

	/* Alpha of widget background color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SubtitlesBackgroundOpacity;

	/* Color of widget background */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SubtitlesBackgroundColor;

	/* Color of dialogue text */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SubtitlesTextColor;

	/* Amount of dialogue and speaker text's outline */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SubtitlesTextOutlineAmount;

	/* Amount of dialogue and speaker text's outline */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> SpeakersDataTable;

	UPROPERTY(config, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> DialogueWidgetBlueprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ZOrder;
};
