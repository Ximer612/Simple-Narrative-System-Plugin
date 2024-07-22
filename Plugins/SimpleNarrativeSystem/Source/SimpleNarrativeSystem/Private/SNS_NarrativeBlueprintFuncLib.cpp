// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#define GET_WORLD_FROM_CONTEXT(World,WorldContextObject) World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

#include "SNS_NarrativeBlueprintFuncLib.h"
#include "Structs/SNS_S_Dialogue.h"
#include "SNS_WorldSubsystem.h"

void USNS_NarrativeBlueprintFuncLib::EnqueueDialogue(UObject* WorldContextObject, const FName DialogueRowName, const UDataTable* DialoguesDataTable)
{
	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	FSNS_S_Dialogue* Dialogue = DialoguesDataTable->FindRow<FSNS_S_Dialogue>(DialogueRowName, "", true);

	UE_LOG(LogTemp, Error, TEXT("Dialogue = %s"), *((Dialogue->TimeStamps[0].SubtitleText).ToString()));

	if (CurrentWorld)
	{
		USNS_WorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_WorldSubsystem>();
		NarrativeSubSystem->OnReceivedDialogue(Dialogue);
		//Dialogue = *NarrativeSubSystem->NarrativeSystem->EnqueueAudio(AudioRowName);
	}




}
