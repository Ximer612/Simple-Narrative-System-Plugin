// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#define GET_WORLD_FROM_CONTEXT(World,WorldContextObject) World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

#include "SNS_NarrativeBlueprintFuncLib.h"
#include "Structs/SNS_S_Dialogue.h"
#include "SNS_DialogueWorldSubsystem.h"

void USNS_NarrativeBlueprintFuncLib::EnqueueDialogue(UObject* WorldContextObject, const FName DialogueRowName, const UDataTable* DialoguesDataTable)
{
	if (DialogueRowName == "" || DialogueRowName == "None")
	{
		checkf(false, TEXT("Invalid subtitle row name!"));
		UE_LOG(LogTemp, Error, TEXT("Invalid subtitle row name!"));
		return;
	}

	//checkf(DialogueRowName != "" || DialogueRowName != "None", TEXT("Dialogue row name cannot be empty!")); throw a blueprint error!

	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	FSNS_S_Dialogue* Dialogue = DialoguesDataTable->FindRow<FSNS_S_Dialogue>(DialogueRowName, "", true);

	if (CurrentWorld)
	{
		USNS_DialogueWorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
		NarrativeSubSystem->EnqueueDialogue(Dialogue);
	}
}
