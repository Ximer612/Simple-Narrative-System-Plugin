// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#define GET_WORLD_FROM_CONTEXT(World,WorldContextObject) World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

#include "SNS_NarrativeBlueprintFuncLib.h"
#include "Structs/SNS_S_Dialogue.h"
#include "SNS_DialogueWorldSubsystem.h"

#define LOCTEXT_NAMESPACE "SNS_NameSpace"

void USNS_NarrativeBlueprintFuncLib::EnqueueDialogue(UObject* WorldContextObject, const FName DialogueRowName, const UDataTable* DialoguesDataTable)
{
	if (DialogueRowName == "" || DialogueRowName == "None")
	{
		FMessageLog("PIE").Error(LOCTEXT("InvalidRow", "Dialogue row name cannot be None/Null!"));
		return;
	}

	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	FSNS_S_Dialogue* Dialogue = DialoguesDataTable->FindRow<FSNS_S_Dialogue>(DialogueRowName, "", true);

	if (!Dialogue)
	{
		FMessageLog("PIE").Error(FText::Format(LOCTEXT("NotFoundRow", "Dialogue row name '{0}' cannot be found!"), FText::FromName(DialogueRowName)));
		return;
	}

	if (CurrentWorld)
	{
		USNS_DialogueWorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
		NarrativeSubSystem->EnqueueDialogue(Dialogue);
	}
}
