// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_NarrativeBlueprintFuncLib.h"
#include "Structs/SNS_S_Dialogue.h"

void USNS_NarrativeBlueprintFuncLib::EnqueueDialogue(const FName DialogueRowName, const UDataTable* DialoguesDataTable)
{
	FSNS_S_Dialogue* Dialogue = DialoguesDataTable->FindRow<FSNS_S_Dialogue>(DialogueRowName, "",true);

	UE_LOG(LogTemp, Error, TEXT("Dialogue = %s"), *((Dialogue->TimeStamps[0].SubtitleText).ToString()) );


}
