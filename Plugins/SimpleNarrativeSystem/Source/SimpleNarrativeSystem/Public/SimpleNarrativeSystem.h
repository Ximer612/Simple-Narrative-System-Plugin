// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSimpleNarrativeSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** Editor Settings **/
	void RegisterSettings();
	void UnRegisterSettings();
};
