// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PinArtToolEditorExtensionCommands.h"

#define LOCTEXT_NAMESPACE "FPinArtToolEditorExtensionModule"

void FPinArtToolEditorExtensionCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "PinArtToolEditorExtension", "Bring up PinArtToolEditorExtension window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
