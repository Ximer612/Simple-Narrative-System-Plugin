// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#include "SNS_DialogueWorldSubsystem.h"
#include "SNS_I_Subtitles.h"
#include "AudioDevice.h"
#include "SNS_NarrativeBlueprintFuncLib.h"

#define LOCTEXT_NAMESPACE "SNS_NameSpace"

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

		CreateSubtitlesWidget(InWorld);

		//IF AUDIO ENABLED?
		CreateAudioComponent(InWorld);

#if WITH_EDITOR
		if (SubtitlesWidget->SpeakersDataTable == nullptr)
		{
			FMessageLog("PIE").Error(LOCTEXT("NullSpeakersDataTable", "Please set a data table for speakers in the widget's properties!"));
			bNoSpeakerDataTable = true;
		}
		else
#endif
			SubtitlesWidget->SpeakersDataTable.LoadSynchronous();
	}
}

void USNS_DialogueWorldSubsystem::CreateSubtitlesWidget(const UWorld& InWorld)
{
	//TODO: find a way to remove this path
	FSoftClassPath MyWidgetClassRef(TEXT("/SimpleNarrativeSystem/UserInterface/WBP_Subtitles.WBP_Subtitles_C"));

	TSubclassOf<USNS_Widget> SubtitlesWidgetClass = MyWidgetClassRef.TryLoadClass<USNS_Widget>();

	if (SubtitlesWidgetClass)
	{
		SubtitlesWidget = Cast<USNS_Widget>(CreateWidget(InWorld.GetFirstPlayerController(), SubtitlesWidgetClass));
		SubtitlesWidget->AddToViewport(612);
		SubtitlesWidget->SpeakersDataTable.LoadSynchronous();
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

	if(SubtitlesWidget)
		SubtitlesWidget->Destruct();
	if (AudioComponent)
		AudioComponent->DestroyComponent();
	
	OnCurrentDialogueEndDelegate.Clear();
	OnCurrentDialogueStartDelegate.Clear();
	OnAllDialoguesEndDelegate.Clear();

	for (TPair<FName,FDialogueEventsLambdas>& Pair : PerDialogueLambdas)
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

	PerDialogueLambdasOnAllEnd.Empty();
	PerDialogueLambdas.Empty();
	DialoguesToPlay.Empty();

	UE_LOG(LogTemp, Error, TEXT("DialoguesToPlay pointer is %p"), &DialoguesToPlay);
	UE_LOG(LogTemp, Error, TEXT("DialoguesToPlay num is %d"), DialoguesToPlay.Num());
	UE_LOG(LogTemp, Error, TEXT("DialoguesToPlay max is %d"), DialoguesToPlay.Max());
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
			if (CurrentDialogueLineIndex != 0 && SubtitlesWidget)
			{
				SubtitlesWidget->OnCurrentLineEnd();
			}
			//if there aren't other timestamps
			if (CurrentDialogueLineIndex >= CurrentDialogue->TimeStamps.Num())
			{
				ManageDialogueEnd(); //check if ended all dialogues or just current
				return;
			}

			if (bShouldAdjustAudioTiming && AudioComponent->Sound != nullptr && CurrentDialogueLineIndex > 0)
			{
				DialogueLineElapsedTime = CurrentDialogue->TimeStamps[CurrentDialogueLineIndex - 1].TimeStamp;
				AudioComponent->Play(DialogueLineElapsedTime);
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



void USNS_DialogueWorldSubsystem::EnqueueDialogue(const FSNS_Dialogue&& InDialogue, const bool bStopAllOtherDialogues)
{
	UE_LOG(LogTemp, Error, TEXT("DialoguesToPlay num is %d"), DialoguesToPlay.Num());

	if (bNoSpeakerDataTable || bIsDisabled || DialoguesToPlay.Contains(InDialogue) )
	{
		return;
	}

	if (bStopAllOtherDialogues)
	{
		DialoguesToPlay.Empty();
		DialoguesToPlay.Add(InDialogue);
		DialogueLineRemaningTime = 0;
		ManageDialogueEnd(false);
		SubtitlesWidget->StopAllOtherDialogues();
		SendDialogue();
		//SubtitlesWidget->OnCurrentLineEnd();
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



void USNS_DialogueWorldSubsystem::CreateAudioComponent(const UWorld& InWorld)
{
	//INIT AUDIO COMPONENT
	FAudioDevice::FCreateComponentParams Params = FAudioDevice::FCreateComponentParams(InWorld.GetAudioDeviceRaw());
	AudioComponent = NewObject<UAudioComponent>((UClass*)Params.AudioComponentClass, "AudioComponent", RF_Standalone);

	AudioComponent->bAutoActivate = false;
	AudioComponent->AttenuationSettings = Params.AttenuationSettings;
	AudioComponent->ConcurrencySet = Params.ConcurrencySet;

	AudioComponent->SetVolumeMultiplier(1.f);
	AudioComponent->SetPitchMultiplier(1.f);
	AudioComponent->bAllowSpatialization = false;
	AudioComponent->bIsUISound = true;
	AudioComponent->bAutoDestroy = false;
	AudioComponent->bIgnoreForFlushing = false; // true or false???
	AudioComponent->bStopWhenOwnerDestroyed = false;

	if (ensure(InWorld.GetWorld()))
	{
		AudioComponent->RegisterComponentWithWorld(InWorld.GetWorld());
	}

	UE_LOG(LogTemp, Error, TEXT("The AudioComponent pointer is %p"), &AudioComponent);
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

		CurrentDialogue->AudioClip.LoadSynchronous();

	if (CurrentDialogue->AudioClip != nullptr)
	{

		if (CurrentDialogue->AudioClip)
		{
			AudioComponent->SetSound(CurrentDialogue->AudioClip.Get());
			AudioComponent->Play(0.f);
		}
	}

	CurrentDialogueLineIndex = 0;

	bIsTickEnabled = true;
	bIsPlayingAudio = true;

	OnCurrentDialogueStartDelegate.Broadcast(CurrentDialogueRowName);

	if (!PerDialogueLambdas.Contains(CurrentDialogueRowName))
	{
		return;
	}

	for (int32 i = 0; i < PerDialogueLambdas[CurrentDialogueRowName].OnStart.Num(); i++)
	{
		if (!PerDialogueLambdas[CurrentDialogueRowName].OnStart[i].bRepeatable)
		{
			OnCurrentDialogueEndDelegate.Remove(PerDialogueLambdas[CurrentDialogueRowName].OnStart[i].DelegateHandle);
			UE_LOG(LogTemp, Error, TEXT("REMOVED HANDLE!"));
		}
	}
}

void USNS_DialogueWorldSubsystem::ManageDialogueEnd(bool bShouldRemoveFirst)
{
	if (bShouldRemoveFirst && !DialoguesToPlay.IsEmpty())
	{
		DialoguesToPlay.RemoveAt(0);
	}

	//stops the audio if the last timestamp dosen't match with it's end
	if (AudioComponent->Sound != nullptr)
	{
		AudioComponent->Stop();
		AudioComponent->Sound = nullptr;
	}

	bIsPlayingAudio = false;

	if (SubtitlesWidget)
	{
		//call ON END CURRENT DIALOGUE LINES (animation to remove subtitle)
		SubtitlesWidget->OnCurrentDialogueEnd();
	}

	OnCurrentDialogueEndDelegate.Broadcast(CurrentDialogueRowName);

	if (PerDialogueLambdas.Contains(CurrentDialogueRowName) )
	{
		for (int32 i = 0; i < PerDialogueLambdas[CurrentDialogueRowName].OnEnd.Num(); i++)
		{
			if (!PerDialogueLambdas[CurrentDialogueRowName].OnEnd[i].bRepeatable)
			{
				OnCurrentDialogueEndDelegate.Remove(PerDialogueLambdas[CurrentDialogueRowName].OnEnd[i].DelegateHandle);
				UE_LOG(LogTemp, Error, TEXT("REMOVED HANDLE!"));
			}
		}
	}

	//if there aren't other dialogues in "queue" to play
	if (DialoguesToPlay.IsEmpty())
	{
		bIsTickEnabled = false;
		//CALL ON END ALL DIALOGUES
		if (SubtitlesWidget)
		{
			SubtitlesWidget->OnAllDialoguesEnd();
		}
		
		OnAllDialoguesEndDelegate.Broadcast(CurrentDialogueRowName);

		for (int32 i = 0; i < PerDialogueLambdasOnAllEnd.Num(); i++)
		{
			if (!PerDialogueLambdasOnAllEnd[i].bRepeatable)
			{
				OnCurrentDialogueEndDelegate.Remove(PerDialogueLambdasOnAllEnd[i].DelegateHandle);
				UE_LOG(LogTemp, Error, TEXT("REMOVED HANDLE!"));
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

	FSNS_S_Speaker* Speaker = SubtitlesWidget->SpeakersDataTable->FindRow<FSNS_S_Speaker>(SpeakerRowName, "", true);

#if WITH_EDITOR
	if (!Speaker)
	{
		FMessageLog("PIE").Error(FText::Format(LOCTEXT("SpeakerNotFound", "Speaker '{0}' cannot be found!"), FText::FromName(SpeakerRowName)));
		UE_LOG(LogTemp, Error, TEXT("SPEAKER %s NOT FOUND!!!!"), *SpeakerRowName.ToString());
		return;
	}
#endif

	DialogueLineRemaningTime += CurrentDialogue->TimeStamps[CurrentDialogueLineIndex].TimeStamp - DialogueLineElapsedTime;

	if (SubtitlesWidget)
	{
		SubtitlesWidget->OnReceivedDialogue(*Speaker, CurrentDialogue->TimeStamps[CurrentDialogueLineIndex]);
	}
}

void USNS_DialogueWorldSubsystem::SkipCurrentLine()
{
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
				UE_LOG(LogTemp, Warning, TEXT("EXECUTED LAMBDA!"));
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
				UE_LOG(LogTemp, Warning, TEXT("EXECUTED LAMBDA!"));
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
			UE_LOG(LogTemp, Warning, TEXT("EXECUTED LAMBDA!"));
		});

	FDialogueLambda DialogueLambda;
	DialogueLambda.bRepeatable = bRepeatable;
	DialogueLambda.DelegateHandle = LambdaHandle;

	PerDialogueLambdasOnAllEnd.Add(DialogueLambda);
}