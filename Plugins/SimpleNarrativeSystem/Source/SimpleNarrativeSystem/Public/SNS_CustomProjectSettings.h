// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

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

	UPROPERTY(config, EditAnywhere, Category = "Settings", meta = (DisplayName = "Speakers' Data Table"))
	TSoftObjectPtr<UDataTable> SpeakersDataTable;

	UPROPERTY(config, EditAnywhere, Category = "Settings", meta = (DisplayName = "Subtitles Widget"))
	TSubclassOf<class UUserWidget> DialogueWidgetBlueprint;
	UPROPERTY(config, EditAnywhere, Category = "Settings", meta = (DisplayName = "Subtitles enabled?"))
	bool bSubtitlesEnabled;

	UPROPERTY(config, VisibleDefaultsOnly, Category = "Info", meta = (DisplayName = "Made by"))
	FString StringSetting;

};

#if UE_ENABLE_INCLUDE_ORDER_DEPRECATED_IN_5_2
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#endif
