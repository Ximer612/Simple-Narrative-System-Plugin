// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleNarrativeSystem.h"

#define LOCTEXT_NAMESPACE "FSimpleNarrativeSystemModule"

void FSimpleNarrativeSystemModule::StartupModule()
{

}



void FSimpleNarrativeSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleNarrativeSystemModule, SimpleNarrativeSystem)