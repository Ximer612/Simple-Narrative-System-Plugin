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

	UPROPERTY(Config, EditAnywhere, Category = "Speakers Settings", meta = (DisplayName = "Enable Simple Narrative System"))
	TSoftObjectPtr<UDataTable> SpeakersDataTable;

	UPROPERTY(Config, VisibleDefaultsOnly, Category = "Info", meta = (EditCondition = bEnableXPlugin, DisplayName = "Made by Ximer"))
	FString StringSetting;

};
