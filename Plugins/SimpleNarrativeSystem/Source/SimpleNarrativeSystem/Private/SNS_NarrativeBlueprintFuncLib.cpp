// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#define GET_WORLD_FROM_CONTEXT(World,WorldContextObject) World = WorldContextObject->GetWorld();

#include "SNS_NarrativeBlueprintFuncLib.h"
#include "Structs/SNS_S_Dialogue.h"
#include "SNS_DialogueWorldSubsystem.h"

#define LOCTEXT_NAMESPACE "SNS_NameSpace"

USNS_DialogueWorldSubsystem* GetNarrativeSubSystem(UObject* WorldContextObject)
{
	/*static*/ USNS_DialogueWorldSubsystem* NarrativeSubSystem;

	//if (NarrativeSubSystem && !NarrativeSubSystem->IsPendingKill())
	//{
	//	//return NarrativeSubSystem;
	//}

	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	if (CurrentWorld)
	{
		NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
		if (NarrativeSubSystem)
		{
			return NarrativeSubSystem;
		}
	}

#if WITH_EDITOR
	FMessageLog("PIE").Error(LOCTEXT("NoNarrativeSubSystem", "Not found Narrative World Sub System! Please contact my developer!"));
#endif

	return nullptr;
}

const FName USNS_NarrativeBlueprintFuncLib::EnqueueDialogue(UObject* WorldContextObject,  const UDataTable* DialoguesDataTable, const FName DialogueRowName, const bool bStopAllOtherDialogues)
{

#if WITH_EDITOR
	if (DialogueRowName == TEXT("") || DialogueRowName == TEXT("None"))
	{
		FMessageLog("PIE").Error(LOCTEXT("InvalidRow", "Dialogue row name cannot be None or null!"));
		return "";
	}
#endif

	GetNarrativeSubSystem(WorldContextObject)->EnqueueDialogue({ DialogueRowName ,DialoguesDataTable }, bStopAllOtherDialogues);

	return DialogueRowName;
}

void USNS_NarrativeBlueprintFuncLib::SkipCurrentDialogueLine(UObject* WorldContextObject)
{
	GetNarrativeSubSystem(WorldContextObject)->SkipCurrentLine();
}

void USNS_NarrativeBlueprintFuncLib::RegisterEventOnEndDialogue(UObject* WorldContextObject, const FName DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueEnd)
{
	GetNarrativeSubSystem(WorldContextObject)->AddOnCurrentDialogueEnd(DialogueRowName, bRepeatable, OnDialogueEnd);
}

void USNS_NarrativeBlueprintFuncLib::RegisterEventOnStartDialogue(UObject* WorldContextObject, const FName DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueStart)
{
	GetNarrativeSubSystem(WorldContextObject)->AddOnCurrentDialogueStart(DialogueRowName, bRepeatable, OnDialogueStart);
}

void USNS_NarrativeBlueprintFuncLib::RegisterEventOnAllDialogueEnd(UObject* WorldContextObject, const bool bRepeatable, const FRegisteredDelegate& OnAllDialoguesEnd)
{
	GetNarrativeSubSystem(WorldContextObject)->AddOnAllCurrentDialogueEnd(bRepeatable, OnAllDialoguesEnd);
}
