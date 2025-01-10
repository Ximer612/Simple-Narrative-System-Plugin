// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleNarrativeSystemEditor.h"
#include "SNS_DataTableCustomization.h"

#define LOCTEXT_NAMESPACE "FSimpleNarrativeSystemEditorModule"

void FSimpleNarrativeSystemEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("SNS_DataTableRowHandle", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSNS_DataTableCustomization::MakeInstance));
}



void FSimpleNarrativeSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout("SNS_DataTableRowHandle");
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleNarrativeSystemEditorModule, SimpleNarrativeSystemEditor)