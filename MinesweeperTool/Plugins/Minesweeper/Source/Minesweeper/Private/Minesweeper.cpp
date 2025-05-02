// Copyright Epic Games, Inc. All Rights Reserved.

#include "Minesweeper.h"
#include "MinesweeperStyle.h"
#include "MinesweeperCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName MinesweeperTabName("Minesweeper");

#define LOCTEXT_NAMESPACE "FMinesweeperModule"

void FMinesweeperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FMinesweeperStyle::Initialize();
	FMinesweeperStyle::ReloadTextures();

	FMinesweeperCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMinesweeperModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperTabName,
	                                                  FOnSpawnTab::CreateRaw(
		                                                  this, &FMinesweeperModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FMinesweeperTabTitle", "Minesweeper"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMinesweeperStyle::Shutdown();

	FMinesweeperCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperTabName);
}

TSharedRef<SDockTab> FMinesweeperModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FMinesweeperModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("Minesweeper.cpp"))
	);

	//Settings Texts
	FText WidthText = FText::FromString(TEXT("Width:"));
	FText HeightText = FText::FromString(TEXT("Height:"));
	FText MinesText = FText::FromString(TEXT("Mines:"));

	//Set up fonts
	const FSlateFontInfo BoldFont = FSlateFontInfo(
		FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 32);
	const FSlateFontInfo RegularFont = FSlateFontInfo(
		FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 32);


	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SVerticalBox)
			+ SVerticalBox::Slot() .Padding(15, 25)
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Font(BoldFont)
					.Text(WidthText)
				]
				
				+ SHorizontalBox::Slot() .Padding(10, 0)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SAssignNew(WidthSetTextBox, SEditableTextBox)
					.Justification(ETextJustify::Left)
					.Font(RegularFont)
					.Text(FText::FromString(TEXT("8")))
				]

				+SHorizontalBox::Slot() .Padding(20, 0)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Font(BoldFont)
					.Text(HeightText)
				]

				+ SHorizontalBox::Slot() .Padding(10, 0)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.AutoWidth()
                [
                	SAssignNew(HeightSetTextBox, SEditableTextBox)
	                .Justification(ETextJustify::Left)
	                .Font(RegularFont)
	                .Text(FText::FromString(TEXT("8")))
                ]

				+ SHorizontalBox::Slot() .Padding(20, 0)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Font(BoldFont)
					.Text(MinesText)
				]

				+SHorizontalBox::Slot() .Padding(10, 0)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SAssignNew(MinesSetTextBox, SEditableTextBox)
					.Justification(ETextJustify::Left)
					.Font(RegularFont)
					.Text(FText::FromString(TEXT("8")))
				]
				
				
			]
		];
}

void FMinesweeperModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
}

void FMinesweeperModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitToolBarButton(FMinesweeperCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMinesweeperModule, Minesweeper)
