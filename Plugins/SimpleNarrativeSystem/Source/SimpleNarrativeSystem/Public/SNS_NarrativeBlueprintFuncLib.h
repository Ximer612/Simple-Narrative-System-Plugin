// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SNS_NarrativeBlueprintFuncLib.generated.h"

DECLARE_DYNAMIC_DELEGATE(FRegisteredDelegate);

/**
 * 
 */
UCLASS()
class SIMPLENARRATIVESYSTEM_API USNS_NarrativeBlueprintFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Simple Narrative System", meta = (DataTablePin = "DialoguesDataTable", RequiredAssetDataTags = "RowStructure=/Script/SimpleNarrativeSystem.SNS_S_Dialogue", WorldContext = "WorldContextObject"))
	static const FName EnqueueDialogue(UObject* WorldContextObject, const UDataTable* DialoguesDataTable, const FName DialogueRowName, UPARAM(DisplayName = "StopAllOtherDialogues?") const bool bStopAllOtherDialogues);

	UFUNCTION(BlueprintCallable, Category = "Simple Narrative System", meta = (WorldContext = "WorldContextObject"))
	static void SkipCurrentDialogueLine(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Simple Narrative System", meta = (WorldContext = "WorldContextObject"))
	static void RegisterEventOnEndDialogue(UObject* WorldContextObject, const FName DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueEnd);
	UFUNCTION(BlueprintCallable, Category = "Simple Narrative System", meta = (WorldContext = "WorldContextObject"))
	static void RegisterEventOnStartDialogue(UObject* WorldContextObject, const FName DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueStart);
	UFUNCTION(BlueprintCallable, Category = "Simple Narrative System", meta = (WorldContext = "WorldContextObject"))
	static void RegisterEventOnAllDialogueEnd(UObject* WorldContextObject, const bool bRepeatable, const FRegisteredDelegate& OnAllDialoguesEnd);


};
