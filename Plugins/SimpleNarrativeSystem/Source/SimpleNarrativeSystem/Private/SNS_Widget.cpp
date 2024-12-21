// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_Widget.h"
#include "SNS_SettingsGameInstanceSS.h"

bool USNS_Widget::Initialize()
{
	Super::Initialize();

	if (!GetWorld()->IsGameWorld())
	{
		return true;
	}

	USNS_SettingsGameInstanceSS* SettingsSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USNS_SettingsGameInstanceSS>();

	SettingsSubsystem->OnSaveSettings.BindLambda(
		[this]() {
			SetStyleFromSettings();			
			return; 
		}
	);

	return true;
}

//void USNS_Widget::NativeConstruct()
//{
//	Super::NativeConstruct();
//}
