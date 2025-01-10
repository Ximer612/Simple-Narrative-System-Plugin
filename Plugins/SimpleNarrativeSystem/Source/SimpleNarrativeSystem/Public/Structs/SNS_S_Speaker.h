// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SNS_S_Speaker.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct SIMPLENARRATIVESYSTEM_API FSNS_S_Speaker : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
    FText SpeakerDisplayedName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
    FLinearColor SpeakerColor = {1.f,1.f,1.f,1.f};

};
