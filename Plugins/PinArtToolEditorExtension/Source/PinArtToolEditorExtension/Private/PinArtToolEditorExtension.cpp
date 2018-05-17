// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PinArtToolEditorExtension.h"
#include "PinArtToolEditorExtensionStyle.h"
#include "PinArtToolEditorExtensionCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Engine.h"
#include "Editor.h"

static const FName PinArtToolEditorExtensionTabName("PinArtToolEditorExtension");

#define LOCTEXT_NAMESPACE "FPinArtToolEditorExtensionModule"

void FPinArtToolEditorExtensionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FPinArtToolEditorExtensionStyle::Initialize();
	FPinArtToolEditorExtensionStyle::ReloadTextures();

	FPinArtToolEditorExtensionCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPinArtToolEditorExtensionCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FPinArtToolEditorExtensionModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FPinArtToolEditorExtensionModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FPinArtToolEditorExtensionModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PinArtToolEditorExtensionTabName, FOnSpawnTab::CreateRaw(this, &FPinArtToolEditorExtensionModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FPinArtToolEditorExtensionTabTitle", "Create Pin Art"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FPinArtToolEditorExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FPinArtToolEditorExtensionStyle::Shutdown();

	FPinArtToolEditorExtensionCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PinArtToolEditorExtensionTabName);
}


void FPinArtToolEditorExtensionModule::GetSizeValues(const FText & Text)
{
	FText TextForString = Text; 
	FString sizeString = TextForString.ToString();
	FString Left, Right;
	sizeString.Split(TEXT("x"), &Left, &Right);
	Left.Replace(TEXT(" "), TEXT(""));
	Right.Replace(TEXT(" "), TEXT(""));
	sizeX = FCString::Atoi(*Left);
	sizeY = FCString::Atoi(*Right);
}

void FPinArtToolEditorExtensionModule::GetImagePath(const FText & Text)
{
	FText TextForString = Text;
	imagePath = TextForString.ToString();
}

FReply FPinArtToolEditorExtensionModule::CreatePinMap()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating Generator"));
	FVector const ObjLocation = FVector::ZeroVector;

	UWorld * world = GEditor->GetEditorWorldContext().World();
	if (world)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is not null"));
		APinGenerator * tempObject = (APinGenerator*)world->SpawnActor(APinGenerator::StaticClass(), &ObjLocation);
		tempObject->CreateTextureValues(imagePath);
		tempObject->Spawn(sizeX, sizeY);
	}

	
	return FReply::Handled();
}

FReply FPinArtToolEditorExtensionModule::ClosePinArtExtension()
{
	FGlobalTabmanager::Get()->GetActiveTab()->GetParentWindow()->EnableWindow(false);
	return FReply::Handled();
}

TSharedRef<SDockTab> FPinArtToolEditorExtensionModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{

	FText CreateArtButtonText = FText::Format(
		LOCTEXT("WindowWidgetText", "{0}"),
		FText::FromString(TEXT("Create Art"))
	);

	FText CancelButtonText = FText::Format(
		LOCTEXT("WindowWidgetText", "{0}"),
		FText::FromString(TEXT("Cancel"))
	);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[

			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.5, 0.5, 0.5, 1.0))
		.ForegroundColor(FLinearColor(0.5, 0.5, 0.5, 1.0))
		[
			SNew(SGridPanel)
			.FillColumn(0, 0.1f)
			.FillColumn(1, 0.1f)


		// SGrid
				+SGridPanel::Slot(0, 0)
				.Padding(2.0, 5.0)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SPinArtOptionsLabel", "Pin Art Options"))
				.ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.9f)) // send color and alpha R G B A
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("ADOBEFANHEITISTD-BOLD.ttf"), 24))
				]

				+ SGridPanel::Slot(0, 1)
					.Padding(2.0, 15.0)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("SDivisionLabel", "_________________________________________________________________________"))
					.ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.9f)) // send color and alpha R G B A
					]

				+ SGridPanel::Slot(0, 2)
				.Padding(2.0, 15.0)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SInstructionsLabel", "Enter a path to an image file and the dimensions"))
					.ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.9f)) // send color and alpha R G B A
				]

			+ SGridPanel::Slot(0, 3)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SNameLabel", "Name"))
					.ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.9f)) // send color and alpha R G B A
				]

			+ SGridPanel::Slot(1, 3)
				.HAlign(HAlign_Left)
				.Padding(0.0f, 1.0f)
				[
					SNew(SEditableTextBox)
					.MinDesiredWidth(400.0f)
				//.HintText(LOCTEXT("SEditableTextHint", "This is editable text"))
				]
			+ SGridPanel::Slot(0, 4)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SImageFileLabel", "Image File"))
					.ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.9f)) // send color and alpha R G B A
				]

			+ SGridPanel::Slot(1, 4)
				.HAlign(HAlign_Left)
				.Padding(0.0f, 1.0f)
				[
					SNew(SEditableTextBox)
					.MinDesiredWidth(400.0f)
					.OnTextChanged_Raw(this, &FPinArtToolEditorExtensionModule::GetImagePath)
				//.HintText(LOCTEXT("SEditableTextHint", "This is editable text"))
				]

			+ SGridPanel::Slot(0, 5)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SSizeLabel", "Size"))
					.ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.9f)) // send color and alpha R G B A
				]

			+ SGridPanel::Slot(1, 5)
				.HAlign(HAlign_Left)
				.Padding(0.0f, 1.0f)
				[
					SNew(SEditableTextBox)
					.MinDesiredWidth(400.0f)
					.OnTextChanged_Raw(this, &FPinArtToolEditorExtensionModule::GetSizeValues)
				//.HintText(LOCTEXT("SEditableTextHint", "This is editable text"))
				]

			+ SGridPanel::Slot(0, 6)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SArtistLabel", "Artist"))
					.ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.9f)) // send color and alpha R G B A
				]

			+ SGridPanel::Slot(1, 6)
				.HAlign(HAlign_Left)
				.Padding(0.0f, 1.0f)
				[
					SNew(SEditableTextBox)
					.MinDesiredWidth(400.0f)
				//.HintText(LOCTEXT("SEditableTextHint", "This is editable text"))
				]

			+ SGridPanel::Slot(0, 7)
				.HAlign(HAlign_Right)
				[
					SNew(SButton)
					.Text(CreateArtButtonText)
					.OnClicked_Raw(this, &FPinArtToolEditorExtensionModule::CreatePinMap)
				]
			+ SGridPanel::Slot(1, 7)
				.HAlign(HAlign_Right)
				[
					SNew(SButton)
					.Text(CancelButtonText)
					.OnClicked_Raw(this, &FPinArtToolEditorExtensionModule::ClosePinArtExtension)
				]
			]
		];
}




void FPinArtToolEditorExtensionModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(PinArtToolEditorExtensionTabName);
}

void FPinArtToolEditorExtensionModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FPinArtToolEditorExtensionCommands::Get().OpenPluginWindow);
}


void FPinArtToolEditorExtensionModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FPinArtToolEditorExtensionCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPinArtToolEditorExtensionModule, PinArtToolEditorExtension)