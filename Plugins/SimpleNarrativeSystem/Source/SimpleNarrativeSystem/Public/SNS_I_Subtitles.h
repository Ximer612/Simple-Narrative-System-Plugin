// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs/SNS_S_Speaker.h"
#include "SNS_I_Subtitles.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USNS_I_Subtitles : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLENARRATIVESYSTEM_API ISNS_I_Subtitles
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnReceivedDialogue(const FSNS_S_Speaker& Speaker, const FText& Text);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAllDialoguesEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCurrentDialogueEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCurrentLineEnd();
};
