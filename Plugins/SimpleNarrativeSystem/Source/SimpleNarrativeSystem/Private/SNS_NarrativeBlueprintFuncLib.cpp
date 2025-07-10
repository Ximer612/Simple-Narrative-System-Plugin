// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#define GET_WORLD_FROM_CONTEXT(World,WorldContextObject) World = WorldContextObject->GetWorld();

#include "SNS_NarrativeBlueprintFuncLib.h"
#include "Structs/SNS_S_Dialogue.h"
#include "SNS_DialogueWorldSubsystem.h"

#if WITH_EDITOR
	#include "SNS_CustomProjectSettings.h"
#endif // WITH_EDITOR

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

const FName USNS_NarrativeBlueprintFuncLib::EnqueueDialogue(UObject* WorldContextObject,  const UDataTable* DialoguesDataTable, const FName DialogueRowName, const bool bStopAllOtherDialogues, bool& bIsValid)
{

#if WITH_EDITOR
	if (DialoguesDataTable->GetRowStruct() != FSNS_S_Dialogue::StaticStruct())
	{
		FMessageLog("PIE").Error(FText::Format(LOCTEXT("WrongDataTableStruct", "Data table '{0}' has the wrong Data Struct! Change it to 'SNS_S_Dialogue'!"), FText::FromString(DialoguesDataTable->GetName())));
		bIsValid = false;
		return "";
	}

	if (DialogueRowName == TEXT("") || DialogueRowName == TEXT("None"))
	{
		bool settingsShowWarning = GetDefault<USNS_CustomProjectSettings>()->bShouldWarningOnNullOrNoneDialogueRowName;
		if(settingsShowWarning)
			FMessageLog("PIE").Warning(LOCTEXT("InvalidRow", "Dialogue row name is None or null!"));

		bIsValid = false;
		return "";
	}

	if (DialoguesDataTable->FindRow<FSNS_S_Dialogue>(DialogueRowName, "", false) == nullptr)
	{
		FMessageLog("PIE").Error(FText::Format(LOCTEXT("DialogueRowNotFound", "Dialogue row name '{0}' cannot be found inside '{1}'!"), FText::FromName(DialogueRowName), FText::FromString(DialoguesDataTable->GetName())));
		bIsValid = false;
		return "";
	}


#endif

	bIsValid = GetNarrativeSubSystem(WorldContextObject)->EnqueueDialogue({ DialogueRowName ,DialoguesDataTable }, bStopAllOtherDialogues);

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

void USNS_NarrativeBlueprintFuncLib::RegisterEventOnDialogueIndex(UObject* WorldContextObject, const FName DialogueRowName, const int32 DialogueRowIndex, const bool bRepeatable, const FRegisteredDelegate& OnDialogueIndex)
{
	GetNarrativeSubSystem(WorldContextObject)->AddOnDialogueIndex(DialogueRowName, bRepeatable, DialogueRowIndex, OnDialogueIndex);
}

void USNS_NarrativeBlueprintFuncLib::PauseCurrentDialogue(UObject* WorldContextObject)
{
	GetNarrativeSubSystem(WorldContextObject)->PauseCurrentDialogue();
}

void USNS_NarrativeBlueprintFuncLib::ResumeCurrentDialogue(UObject* WorldContextObject)
{
	GetNarrativeSubSystem(WorldContextObject)->ResumeCurrentDialogue();
}