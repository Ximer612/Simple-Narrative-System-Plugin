// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#define GET_WORLD_FROM_CONTEXT(World,WorldContextObject) World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

#include "SNS_NarrativeBlueprintFuncLib.h"
#include "Structs/SNS_S_Dialogue.h"
#include "SNS_DialogueWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

#include "SNS_I_Subtitles.h"

#define LOCTEXT_NAMESPACE "SNS_NameSpace"

void USNS_NarrativeBlueprintFuncLib::EnqueueDialogue(UObject* WorldContextObject, const FName DialogueRowName, const UDataTable* DialoguesDataTable, const bool bStopAllOtherDialogues)
{

#if WITH_EDITOR
	if (DialogueRowName == TEXT("") || DialogueRowName == TEXT("None"))
	{
		FMessageLog("PIE").Error(LOCTEXT("InvalidRow", "Dialogue row name cannot be None or null!"));
		return;
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
