// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "PinArtToolEditorExtensionStyle.h"

class FPinArtToolEditorExtensionCommands : public TCommands<FPinArtToolEditorExtensionCommands>
{
public:

	FPinArtToolEditorExtensionCommands()
		: TCommands<FPinArtToolEditorExtensionCommands>(TEXT("PinArtToolEditorExtension"), NSLOCTEXT("Contexts", "PinArtToolEditorExtension", "PinArtToolEditorExtension Plugin"), NAME_None, FPinArtToolEditorExtensionStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};