// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#define GET_WORLD_FROM_CONTEXT(World,WorldContextObject) World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

#define ADD_DIALOGUE_DELEGATE(DialogueRowName,OnDialogueDelegate,NarrativeSubSystem) FDelegateHandle Handle = NarrativeSubSystem->OnCurrentDialogueEndDelegate.AddLambda(\
[DialogueRowName, OnDialogue, NarrativeSubSystem, Handle](FName DialogueName) {\
	if (DialogueName == DialogueRowName)\
	{\
		OnDialogueEnd.ExecuteIfBound();\
		NarrativeSubSystem->OnCurrentDialogueEndDelegate.Remove(Handle);\
	}\
	});\
\

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

	if (CurrentWorld)
	{
		USNS_DialogueWorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
		if (NarrativeSubSystem)
		{
			FDelegateHandle* LambdaHandle = new FDelegateHandle();
			*LambdaHandle = NarrativeSubSystem->OnCurrentDialogueEndDelegate.AddLambda(
				[DialogueRowName, OnDialogueEnd, LambdaHandle, NarrativeSubSystem, bRepeatable](FName DialogueName) {
					if (DialogueName == DialogueRowName)
					{
						OnDialogueEnd.ExecuteIfBound();
						UE_LOG(LogTemp, Warning, TEXT("EXECUTED HANDLE!"));
						if (!bRepeatable)
						{
							NarrativeSubSystem->OnCurrentDialogueEndDelegate.Remove(*LambdaHandle);
							UE_LOG(LogTemp, Error, TEXT("REMOVED HANDLE! %p"), LambdaHandle);
							delete LambdaHandle;
						}
					}				
				});
		}
	}
}


//if (!bRepeatable)
//{
//	NarrativeSubSystem->OnCurrentDialogueEndDelegate.AddLambda(
//		[DialogueRowName, NarrativeSubSystem,Handle](FName DialogueName) {
//			if (DialogueName == DialogueRowName)
//			{
//				NarrativeSubSystem->OnCurrentDialogueEndDelegate.Remove(Handle);
//				UE_LOG(LogTemp, Error, TEXT("REMOVED HANDLE!"));
//			}
//		});
//}

void USNS_NarrativeBlueprintFuncLib::RegisterEventOnStartDialogue(UObject* WorldContextObject, const FName DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueStart)
{
	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	if (CurrentWorld)
	{
		USNS_DialogueWorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
		if (NarrativeSubSystem)
		{
			FDelegateHandle Handle = NarrativeSubSystem->OnCurrentDialogueStartDelegate.AddLambda(
				[DialogueRowName, OnDialogueStart](FName DialogueName) {
					if (DialogueName == DialogueRowName)
					{
						OnDialogueStart.ExecuteIfBound();
					}
				});
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
			FDelegateHandle Handle = NarrativeSubSystem->OnAllDialoguesEndDelegate.AddLambda(
				[OnAllDialoguesEnd](FName DialogueName) {
						OnAllDialoguesEnd.ExecuteIfBound();
				});
		}
	}
}
