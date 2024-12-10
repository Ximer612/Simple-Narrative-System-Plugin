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
	if (InWorld.IsGameWorld())
	{
		UGameInstance* GameInstance = InWorld.GetGameInstance();

		if (!GameInstance)
		{
			return;
		}

		InGameManager = InWorld.SpawnActor<ASNS_Manager>();
	}
}
USNS_DialogueWorldSubsystem::~USNS_DialogueWorldSubsystem()
{
}
void USNS_DialogueWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	bIsTickEnabled = false;
	bIsPlayingAudio = false;
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
			//if it's not the first subtitle line
			if (CurrentDialogueLineIndex != 0 && InGameManager->SubtitlesWidget)
			{
				InGameManager->SubtitlesWidget->OnCurrentLineEnd();
			}
			//if there aren't other timestamps
			if (CurrentDialogueLineIndex >= CurrentDialogue->TimeStamps.Num())
			{
				ManageDialogueEnd(); //check if ended all dialogues or just current
				return;
			}

			if (bShouldAdjustAudioTiming && InGameManager->AudioComponent->Sound != nullptr && CurrentDialogueLineIndex > 0)
			{
				DialogueLineElapsedTime = CurrentDialogue->TimeStamps[CurrentDialogueLineIndex - 1].TimeStamp;
				InGameManager->AudioComponent->Play(DialogueLineElapsedTime);
				bShouldAdjustAudioTiming = false;
			}

			SendDialogue();

			CurrentDialogueLineIndex++;
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

		for (int32 i = 0; i < DialoguesToPlay.Num(); i++)
		{
			OnCurrentDialogueStartDelegate.Broadcast(DialoguesToPlay[i].DialogueRowName);
			OnCurrentDialogueEndDelegate.Broadcast(DialoguesToPlay[i].DialogueRowName);
		}

		//Not calling this because if the "important" dialogue we skipo the other but not all because we are inside this array too
		//OnAllDialoguesEndDelegate.Broadcast(CurrentDialogueRowName);

		DialoguesToPlay.Empty();
		DialoguesToPlay.Add(InDialogue);

		DialogueLineRemaningTime = 0;
		ManageDialogueEnd(false);

		InGameManager->SubtitlesWidget->StopAllOtherDialogues();

		SendDialogue();
	}
	else
	{
		DialoguesToPlay.Add(InDialogue);
	}


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

void USNS_DialogueWorldSubsystem::SendDialogue()
{
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
}

void USNS_DialogueWorldSubsystem::SkipCurrentLine()
{
	if (!CurrentDialogue->bCanBeSkipped)
	{
		return;
	}

	DialogueLineRemaningTime = 0;
	bShouldAdjustAudioTiming = true;
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