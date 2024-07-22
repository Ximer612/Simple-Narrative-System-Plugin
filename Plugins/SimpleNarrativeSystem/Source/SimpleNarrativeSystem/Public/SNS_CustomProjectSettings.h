// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SNS_CustomProjectSettings.generated.h"

/**
 * 
 */
UCLASS(config = SimpleNarrativeSystem, defaultconfig)
class SIMPLENARRATIVESYSTEM_API USNS_CustomProjectSettings : public UObject
{
	GENERATED_BODY()
	
public:
	USNS_CustomProjectSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Config, EditAnywhere, Category = "Settings", meta = (DisplayName = "Speakers' Data Table"))
	TSoftObjectPtr<UDataTable> SpeakersDataTable;

	UPROPERTY(Config, EditAnywhere, Category = "Settings", meta = (DisplayName = "Subtitles Widget"))
	TSubclassOf<class UUserWidget> DialogueWidgetBlueprint;

	UPROPERTY(Config, VisibleDefaultsOnly, Category = "Info", meta = (DisplayName = "Made by"))
	FString StringSetting;

};
