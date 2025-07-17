// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_Manager.h"
#include "SNS_Widget.h"
#include "SNS_CustomProjectSettings.h"
#define GET_SETTINGS GetDefault<USNS_CustomProjectSettings>()


// Sets default values
ASNS_Manager::ASNS_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;	

	SubtitlesWidget = CreateDefaultSubobject<USNS_Widget>("SNS_Widget");
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("SNS_AudioComponent");

	RootComponent = AudioComponent;
	AudioComponent->bIsUISound = false;

}

// Called when the game starts or when spawned
void ASNS_Manager::BeginPlay()
{
	Super::BeginPlay();
	
	FSoftClassPath MyWidgetClassRef(GET_SETTINGS->DialogueWidgetBlueprint);
	TSubclassOf<USNS_Widget> SubtitlesWidgetClass = MyWidgetClassRef.TryLoadClass<USNS_Widget>();

	const TSoftObjectPtr<USoundClass>& DialoguesSoundClass = GET_SETTINGS->DialoguesSoundClass;

	//TObjectPtr<USoundClass> MySoundClassRef(GET_SETTINGS->DialoguesSoundClass);
	
	if (GEngine && DialoguesSoundClass.IsNull())
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Is null!"));

	if (!DialoguesSoundClass.IsNull())
	{
		AudioComponent->SoundClassOverride = DialoguesSoundClass.LoadSynchronous();
	}

	if (MyWidgetClassRef.IsNull() || SubtitlesWidgetClass == nullptr)
	{
		FMessageLog("PIE").Error(FText::FromString(TEXT("Can't find a Subtitle Widget, please set it inside 'Project Settings/Plugins/Simple Narrative System/Subtitles Widget'.")));
		bHasValidWidget = false;
		return;
	}

	SubtitlesWidget = Cast<USNS_Widget>(CreateWidget(GetWorld()->GetFirstPlayerController(), SubtitlesWidgetClass));
	SubtitlesWidget->AddToViewport(612);

}

// Called every frame
void ASNS_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

