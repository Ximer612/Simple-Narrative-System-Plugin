// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SNS_NarrativeBlueprintFuncLib.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLENARRATIVESYSTEM_API USNS_NarrativeBlueprintFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintCallable, meta = (DataTablePin = "/Script/SimpleNarrativeSystem.SNS_S_Dialogue", WorldContext = "WorldContextObject"))
	static void EnqueueDialogue(UObject* WorldContextObject, const FName DialogueRowName, const UDataTable* DialoguesDataTable, UPARAM(DisplayName = "StopAllOtherDialogues?") const bool bStopAllOtherDialogues);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void SkipCurrentDialogueLine(UObject* WorldContextObject);
};
