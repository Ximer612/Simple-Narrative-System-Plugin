// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_CustomProjectSettings.h"

USNS_CustomProjectSettings::USNS_CustomProjectSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, DialogueWidgetBlueprint(nullptr)
, PluginInfo("Ximer - Marco Baldini - 2024")
{
	//Start with the default widget already put inside the Project Settings
	FSoftClassPath MyWidfgetClassRef;
	MyWidfgetClassRef.SetPath(TEXT("/SimpleNarrativeSystem/UserInterface/WBP_SNS_Subtitles.WBP_SNS_Subtitles_C"));
	DialogueWidgetBlueprint = MyWidfgetClassRef.TryLoadClass<USNS_Widget>();
}
