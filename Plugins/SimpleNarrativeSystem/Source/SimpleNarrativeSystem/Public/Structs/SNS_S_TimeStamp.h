// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "../SNS_DataTableRowHandle.h"
#include "SNS_S_TimeStamp.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct SIMPLENARRATIVESYSTEM_API FSNS_S_TimeStamp
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "/Script/SimpleNarrativeSystem.SNS_S_Speaker"), Category = "SimpleNarrativeSystem")
	FSNS_DataTableRowHandle Speaker;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	float SpeakerTopPadding = 0;

	// default value for a single line dialogue TODO: every time you add a new entry in a data table increase this number by the previous
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	float TimeStamp = 3.f; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	FText SubtitleText;


};
