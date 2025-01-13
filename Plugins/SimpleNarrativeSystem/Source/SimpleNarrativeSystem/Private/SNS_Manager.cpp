// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_Manager.h"
#include "SNS_Widget.h"

// Sets default values
ASNS_Manager::ASNS_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;	

	SubtitlesWidget = CreateDefaultSubobject<USNS_Widget>("SNS_Widget");
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("SNS_AudioComponent");

	RootComponent = AudioComponent;
	AudioComponent->bIsUISound = true;

}

// Called when the game starts or when spawned
void ASNS_Manager::BeginPlay()
{
	Super::BeginPlay();
	
	FSoftClassPath MyWidgetClassRef(TEXT("/SimpleNarrativeSystem/UserInterface/WBP_SNS_Subtitles.WBP_SNS_Subtitles_C"));
	TSubclassOf<USNS_Widget> SubtitlesWidgetClass = MyWidgetClassRef.TryLoadClass<USNS_Widget>();

	if (MyWidgetClassRef.IsNull() || SubtitlesWidgetClass == nullptr)
	{
		FMessageLog("PIE").Error(FText::FromString(TEXT("Can't find Subtitle Widget, please restore it inside '/SimpleNarrativeSystem/UserInterface/' named 'WBP_SNS_Subtitles'.")));
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

