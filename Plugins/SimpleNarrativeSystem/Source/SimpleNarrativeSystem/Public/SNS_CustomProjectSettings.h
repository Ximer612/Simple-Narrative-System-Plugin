// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#if UE_ENABLE_INCLUDE_ORDER_DEPRECATED_IN_5_2
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#endif

#include "UObject/SoftObjectPath.h"
#include "SNS_Widget.h"
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

	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (DisplayName = "Subtitles Widget"))
	TSubclassOf<USNS_Widget> DialogueWidgetBlueprint;
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSoftObjectPtr<class USoundClass> DialoguesSoundClass;
	UPROPERTY(config, EditAnywhere, Category = "Settings")
	int32 WidgetZOrder = 612;
	UPROPERTY(config, EditAnywhere, Category = "Settings", meta = (DisplayName = "Should warn when row name is null or none?"))
	bool bShouldWarningOnNullOrNoneDialogueRowName;
	UPROPERTY(config, VisibleDefaultsOnly, Category = "Info", meta = (DisplayName = "Made by"))
	FString PluginInfo;

};


