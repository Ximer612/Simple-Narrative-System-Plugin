// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#define GET_WORLD_FROM_CONTEXT(World,WorldContextObject) World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

#include "SNS_NarrativeBlueprintFuncLib.h"
#include "Structs/SNS_S_Dialogue.h"
#include "SNS_DialogueWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/RichTextBlock.h"
#include "SNS_I_Subtitles.h"
#include "Fonts/FontMeasure.h"

#define LOCTEXT_NAMESPACE "SNS_NameSpace"

//USNS_DialogueWorldSubsystem* NarrativeSubSystems;

const FName USNS_NarrativeBlueprintFuncLib::EnqueueDialogue(UObject* WorldContextObject,  const UDataTable* DialoguesDataTable, const FName DialogueRowName, const bool bStopAllOtherDialogues)
{

#if WITH_EDITOR
	if (DialogueRowName == TEXT("") || DialogueRowName == TEXT("None"))
	{
		FMessageLog("PIE").Error(LOCTEXT("InvalidRow", "Dialogue row name cannot be None or null!"));
		return "";
	}
#endif

	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	if (CurrentWorld)
	{
		USNS_DialogueWorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
		if (NarrativeSubSystem)
		{
			NarrativeSubSystem->EnqueueDialogue({ DialogueRowName ,DialoguesDataTable}, bStopAllOtherDialogues);
		}
	}

	return DialogueRowName;
}

void USNS_NarrativeBlueprintFuncLib::SkipCurrentDialogueLine(UObject* WorldContextObject)
{
	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	if (CurrentWorld)
	{
		USNS_DialogueWorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
		if (NarrativeSubSystem)
		{
			NarrativeSubSystem->SkipCurrentLine();
		}
	}

}

void USNS_NarrativeBlueprintFuncLib::RegisterEventOnEndDialogue(UObject* WorldContextObject, const FName DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueEnd)
{
	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	if (!CurrentWorld)
	{
		return;
	}

	USNS_DialogueWorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
	if (NarrativeSubSystem)
	{
		NarrativeSubSystem->AddOnCurrentDialogueEnd(DialogueRowName, bRepeatable, OnDialogueEnd);
	}
}

void USNS_NarrativeBlueprintFuncLib::RegisterEventOnStartDialogue(UObject* WorldContextObject, const FName DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueStart)
{
	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	if (CurrentWorld)
	{
		USNS_DialogueWorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
		if (NarrativeSubSystem)
		{
			NarrativeSubSystem->AddOnCurrentDialogueStart(DialogueRowName, bRepeatable, OnDialogueStart);
		}
	}
}

void USNS_NarrativeBlueprintFuncLib::RegisterEventOnAllDialogueEnd(UObject* WorldContextObject, const bool bRepeatable, const FRegisteredDelegate& OnAllDialoguesEnd)
{
	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	if (CurrentWorld)
	{
		USNS_DialogueWorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
		if (NarrativeSubSystem)
		{
			NarrativeSubSystem->AddOnAllCurrentDialogueEnd(bRepeatable, OnAllDialoguesEnd);
		}
	}
}
