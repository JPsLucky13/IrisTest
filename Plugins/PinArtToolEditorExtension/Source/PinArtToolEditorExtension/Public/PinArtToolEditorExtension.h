// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "../Source/IrisTest/Public/PinGenerator.h"

class FToolBarBuilder;
class FMenuBuilder;

class FPinArtToolEditorExtensionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);
	void GetSizeValues(const FText & Text);
	void GetImagePath(const FText & Text);

	FReply CreatePinMap();
	FReply ClosePinArtExtension();

	int sizeX = 0;
	int sizeY = 0;

	FString imagePath;

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};