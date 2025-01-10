// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#include "SNS_DialogueWorldSubsystem.h"
#include "SNS_I_Subtitles.h"
#include "SNS_NarrativeBlueprintFuncLib.h"

#define LOCTEXT_NAMESPACE "SNS_NameSpace"

#pragma region Inherited methods

USNS_DialogueWorldSubsystem::USNS_DialogueWorldSubsystem()
{

}
void USNS_DialogueWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{

}
USNS_DialogueWorldSubsystem::~USNS_DialogueWorldSubsystem()
{
}
void USNS_DialogueWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	bIsTickEnabled = false;
	bIsPlayingAudio = false;

	if (GetWorld() && GetWorld()->IsGameWorld())
	{
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();

		if (!GameInstance)
		{
			return;
		}
		bTESTVARIABLEREMOVEME = false;
		InGameManager = GetWorld()->SpawnActor<ASNS_Manager>();
	}
}
void USNS_DialogueWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	OnCurrentDialogueEndDelegate.Clear();
	OnCurrentDialogueStartDelegate.Clear();
	OnAllDialoguesEndDelegate.Clear();

	ClearTMap();
	
	DialoguesToPlay.Empty();
}

void USNS_DialogueWorldSubsystem::Tick(float DeltaTime)
{
	if (bIsTickEnabled)
	{
		DialogueLineElapsedTime += DeltaTime;
		DialogueLineRemaningTime -= DeltaTime;

		//if remaning time is over
		if (DialogueLineRemaningTime < 0)
		{
			//it notifies when the previous dialogue is ended
			if (/*CurrentDialogueLineIndex != 0 &&*/ InGameManager->SubtitlesWidget)
			{
				InGameManager->SubtitlesWidget->OnCurrentLineEnd();
			}

			CurrentDialogueLineIndex++;

			//if there aren't other timestamps
			if (CurrentDialogueLineIndex >= CurrentDialogue->TimeStamps.Num())
			{
				ManageDialogueEnd(); //check if ended all dialogues or just current
				return;
			}


			if (bShouldAdjustAudioTiming && InGameManager->AudioComponent->Sound != nullptr)
			{
				bShouldAdjustAudioTiming = false;
				DialogueLineElapsedTime = CurrentDialogue->TimeStamps[CurrentDialogueLineIndex-1].TimeStamp; // -1 because the time elapsed is the duration time of the previous dialogue line
				InGameManager->AudioComponent->Play(DialogueLineElapsedTime);
			}

			SendDialogueToWidget();
			
		}
	}

}
TStatId USNS_DialogueWorldSubsystem::GetStatId() const
{
	return StatId;
}

#pragma endregion

void USNS_DialogueWorldSubsystem::EnqueueDialogue(const FSNS_Dialogue&& InDialogue, const bool bStopAllOtherDialogues)
{
	if (bNoSpeakerDataTable || bIsDisabled || DialoguesToPlay.Contains(InDialogue) )
	{
		return;
	}

	if (bStopAllOtherDialogues)
	{
		//CALL ALL EVENTS OF THE DIALOGUES TO PLAY

		int32 dialoguesToPlayNum = DialoguesToPlay.Num();

		if (dialoguesToPlayNum > 0)
		{
			for (int32 i = 0; i < dialoguesToPlayNum; i++)
			{
				OnCurrentDialogueStartDelegate.Broadcast(DialoguesToPlay[i].DialogueRowName);
				OnCurrentDialogueEndDelegate.Broadcast(DialoguesToPlay[i].DialogueRowName);
			}

			DialoguesToPlay.Empty();
			InGameManager->SubtitlesWidget->StopAllOtherDialogues();
		}

		//Not calling this because if the "important" dialogue we skip the other but not all because we are inside this array too
		//OnAllDialoguesEndDelegate.Broadcast(CurrentDialogueRowName);

		DialoguesToPlay.Add(InDialogue);

		DialogueLineRemaningTime = 0;
		ManageDialogueEnd(false);
	}
	else
	{
		DialoguesToPlay.Add(InDialogue);
	}

	//SendDialogueToWidget();

	if (!bIsPlayingAudio)
	{
		bool AllLinesEnded;
		PlayDialogue(AllLinesEnded);
	}
}

void USNS_DialogueWorldSubsystem::PlayDialogue(bool& AllLinesEnded)
{
	if (DialoguesToPlay.IsEmpty())
	{
		AllLinesEnded = true;
		return;
	}

	AllLinesEnded = false;

	DialogueLineElapsedTime = 0;
	CurrentDialogueRowName = DialoguesToPlay[0].DialogueRowName;
	FSNS_S_Dialogue* TempDialogue = DialoguesToPlay[0].DialoguesDataTable->FindRow<FSNS_S_Dialogue>(CurrentDialogueRowName, "", false);

	if (!TempDialogue)
	{
		FMessageLog("PIE").Error(FText::Format(LOCTEXT("NotFoundRow", "Dialogue row name '{0}' cannot be found!"), FText::FromName(DialoguesToPlay[0].DialogueRowName)));
		return;
	}

	CurrentDialogue = TempDialogue;

	if (!CurrentDialogue->AudioClip.IsNull())
	{
		CurrentDialogue->AudioClip.LoadSynchronous();
	}

	if (CurrentDialogue->AudioClip != nullptr)
	{

		if (CurrentDialogue->AudioClip)
		{
			InGameManager->AudioComponent->SetSound(CurrentDialogue->AudioClip.Get());
			InGameManager->AudioComponent->Play(0.f);
		}
	}

	CurrentDialogueLineIndex = 0;

	bIsTickEnabled = true;
	bIsPlayingAudio = true;

	SendDialogueToWidget();

	CallDialogueDelegate(OnCurrentDialogueStartDelegate, CurrentDialogueRowName,true);
}

void USNS_DialogueWorldSubsystem::ManageDialogueEnd(bool bShouldRemoveFirst)
{
	if (bShouldRemoveFirst && !DialoguesToPlay.IsEmpty())
	{
		DialoguesToPlay.RemoveAt(0);
	}

	//stops the audio if the last timestamp dosen't match with it's end
	if (InGameManager->AudioComponent->Sound != nullptr)
	{
		InGameManager->AudioComponent->Stop();
		InGameManager->AudioComponent->Sound = nullptr;
	}

	bIsPlayingAudio = false;

	if (InGameManager->SubtitlesWidget)
	{
		//call ON END CURRENT DIALOGUE LINES (animation to remove subtitle)
		InGameManager->SubtitlesWidget->OnCurrentDialogueEnd();
	}

	CallDialogueDelegate(OnCurrentDialogueEndDelegate, CurrentDialogueRowName,false);

	//if there aren't other dialogues in "queue" to play
	if (DialoguesToPlay.IsEmpty())
	{
		bIsTickEnabled = false;
		//CALL ON END ALL DIALOGUES
		if (InGameManager->SubtitlesWidget)
		{
			InGameManager->SubtitlesWidget->OnAllDialoguesEnd();
		}
		
		OnAllDialoguesEndDelegate.Broadcast(CurrentDialogueRowName);

		for (int32 i = 0; i < PerDialogueLambdasOnAllEnd.Num(); i++)
		{
			if (!PerDialogueLambdasOnAllEnd[i].bRepeatable)
			{
				OnCurrentDialogueEndDelegate.Remove(PerDialogueLambdasOnAllEnd[i].DelegateHandle);
			}
		}
	}
	else
	{
		//play next queue dialogue
		bool AllDialoguesEnded;
		PlayDialogue(AllDialoguesEnded);
		bIsTickEnabled = true;
	}
}

void USNS_DialogueWorldSubsystem::SendDialogueToWidget()
{
	if (bTESTVARIABLEREMOVEME)
	{
		UE_LOG(LogTemp, Warning, TEXT("DEVI CHIAMARE PRIMA IL BEGIN PLAY?"));
	}

	const FName& SpeakerRowName = CurrentDialogue->TimeStamps[CurrentDialogueLineIndex].Speaker.RowName;

	FSNS_S_Speaker* Speaker = CurrentDialogue->TimeStamps[CurrentDialogueLineIndex].Speaker.DataTable->FindRow<FSNS_S_Speaker>(SpeakerRowName, "", true);

#if WITH_EDITOR
	if (!Speaker)
	{
		FMessageLog("PIE").Error(FText::Format(LOCTEXT("SpeakerNotFound", "Speaker '{0}' cannot be found!"), FText::FromName(SpeakerRowName)));
		return;
	}
#endif

	DialogueLineRemaningTime += CurrentDialogue->TimeStamps[CurrentDialogueLineIndex].TimeStamp - DialogueLineElapsedTime;

	if (InGameManager->SubtitlesWidget)
	{
		InGameManager->SubtitlesWidget->OnReceivedDialogue(*Speaker, CurrentDialogue->TimeStamps[CurrentDialogueLineIndex], CurrentDialogue->bCanBeSkipped);
	}
	else {
		UE_LOG(LogTemp,Error,TEXT("Subtitle Widget cannot be found! Please contact the developer"))
	}
}

void USNS_DialogueWorldSubsystem::SkipCurrentLine()
{
	if (CurrentDialogue == nullptr || !CurrentDialogue->bCanBeSkipped)
	{
		return;
	}

	DialogueLineRemaningTime = 0;

	if (InGameManager->AudioComponent->Sound != nullptr && DialogueLineElapsedTime < InGameManager->AudioComponent->Sound->GetDuration())
	{
		bShouldAdjustAudioTiming = true;
	}
}

void USNS_DialogueWorldSubsystem::CheckDialogueMapContainsRowName(const FName& DialogueRowName)
{
	if (!PerDialogueLambdas.Contains(DialogueRowName))
	{
		PerDialogueLambdas.Add(DialogueRowName, FDialogueEventsLambdas()); //without move temp?
	}

}

void USNS_DialogueWorldSubsystem::AddOnCurrentDialogueEnd(const FName& DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueEnd)
{
	CheckDialogueMapContainsRowName(DialogueRowName);

	FDelegateHandle LambdaHandle = OnCurrentDialogueEndDelegate.AddLambda(
		[DialogueRowName, OnDialogueEnd](FName DialogueName) {
			if (DialogueName == DialogueRowName)
			{
				OnDialogueEnd.ExecuteIfBound();
			}
		});

	FDialogueLambda DialogueLambda;
	DialogueLambda.bRepeatable = bRepeatable;
	DialogueLambda.DelegateHandle = LambdaHandle;

	PerDialogueLambdas[DialogueRowName].OnEnd.Add(DialogueLambda); // move? unique?
}

void USNS_DialogueWorldSubsystem::AddOnCurrentDialogueStart(const FName& DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueStart)
{
	CheckDialogueMapContainsRowName(DialogueRowName);

	FDelegateHandle LambdaHandle = OnCurrentDialogueStartDelegate.AddLambda(
		[DialogueRowName, OnDialogueStart](FName DialogueName) {
			if (DialogueName == DialogueRowName)
			{
				OnDialogueStart.ExecuteIfBound();
			}
		});

	FDialogueLambda DialogueLambda;
	DialogueLambda.bRepeatable = bRepeatable;
	DialogueLambda.DelegateHandle = LambdaHandle;

	PerDialogueLambdas[DialogueRowName].OnStart.Add(DialogueLambda); // move? unique?
}

void USNS_DialogueWorldSubsystem::AddOnAllCurrentDialogueEnd(const bool bRepeatable, const FRegisteredDelegate& OnAllDialogueEnd)
{
	FDelegateHandle LambdaHandle = OnAllDialoguesEndDelegate.AddLambda(
		[OnAllDialogueEnd](FName DialogueName) {
			OnAllDialogueEnd.ExecuteIfBound();
		});

	FDialogueLambda DialogueLambda;
	DialogueLambda.bRepeatable = bRepeatable;
	DialogueLambda.DelegateHandle = LambdaHandle;

	PerDialogueLambdasOnAllEnd.Add(DialogueLambda);
}

void USNS_DialogueWorldSubsystem::ClearTMap()
{
	for (TPair<FName, FDialogueEventsLambdas>& Pair : PerDialogueLambdas)
	{
		for (size_t i = 0; i < Pair.Value.OnEnd.Num(); i++)
		{
			Pair.Value.OnEnd[i].DelegateHandle.Reset();
		}

		for (size_t i = 0; i < Pair.Value.OnStart.Num(); i++)
		{
			Pair.Value.OnEnd[i].DelegateHandle.Reset();
		}
	}

	for (size_t i = 0; i < PerDialogueLambdasOnAllEnd.Num(); i++)
	{
		PerDialogueLambdasOnAllEnd[i].DelegateHandle.Reset();
	}

	PerDialogueLambdas.Empty();
	PerDialogueLambdasOnAllEnd.Empty();
}

void USNS_DialogueWorldSubsystem::CallDialogueDelegate(FOnDialogueDelegate& InDialogueDelegate, const FName& InDialogueRowName, const bool bOnStart)
{
	InDialogueDelegate.Broadcast(InDialogueRowName);

	if (!PerDialogueLambdas.Contains(InDialogueRowName))
	{
		return;
	}

	if (bOnStart)
	{
		for (int32 i = 0; i < PerDialogueLambdas[InDialogueRowName].OnStart.Num(); i++)
		{
			if (!PerDialogueLambdas[CurrentDialogueRowName].OnStart[i].bRepeatable)
			{
				InDialogueDelegate.Remove(PerDialogueLambdas[InDialogueRowName].OnStart[i].DelegateHandle);
			}
		}
	}
	else 
	{
		for (int32 i = 0; i < PerDialogueLambdas[InDialogueRowName].OnEnd.Num(); i++)
		{
			if (!PerDialogueLambdas[CurrentDialogueRowName].OnEnd[i].bRepeatable)
			{
				InDialogueDelegate.Remove(PerDialogueLambdas[InDialogueRowName].OnEnd[i].DelegateHandle);
			}
		}
	}
}