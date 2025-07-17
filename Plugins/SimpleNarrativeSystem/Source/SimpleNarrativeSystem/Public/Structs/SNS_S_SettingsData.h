// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SNS_S_SettingsData.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct SIMPLENARRATIVESYSTEM_API FSNS_S_SettingsData
{
	GENERATED_BODY()

	/* Should show subtitles? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	bool bSubtitlesEnabled = true;

	/* Should show who is speaking? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	bool bSpeakerNameEnabled = true;

	/* Text size */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SimpleNarrativeSystem")
	float SubtitlesSize = 1.f;

	/* Color of widget background with alpha */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	FLinearColor SubtitlesBackgroundColor = {1.f,1.f,1.f,1.f};

	/* Color of dialogue text */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	FLinearColor SubtitlesTextColor = { 0.f,0.f,0.f,1.f };

	/* Should have a second/fade out text? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	bool bSecondTextEnabled = true;

	/* Should fade out texts? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	bool bFadeOutEnabled = true;

	// not working with rich text
	/* Amount of dialogue and speaker text's outline */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//float SubtitlesTextOutlineAmount;
};
