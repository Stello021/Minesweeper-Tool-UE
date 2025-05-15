// Copyright Epic Games, Inc. All Rights Reserved.

#include "Minesweeper.h"
#include "MinesweeperStyle.h"
#include "MinesweeperCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "MinesweeperButton.h"


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
	//Define constants coputed at compile time (constexpr)
	//This way allows type-safety, maintain scope rules and debugging support unlike macros
	//Why not constinit? These values shouldn't change during program execution, C++ 11 support
	constexpr int32 DefaultGridWidth = 8;
	constexpr int32 DefaultGridHeight = 8;
	constexpr int32 DefaultGridMines = 8;
	constexpr float LabelPadding = 20.f;
	constexpr float InputPadding = 10.f;


	//Settings Texts
	FText WidthText = FText::FromString(TEXT("Width:"));
	FText HeightText = FText::FromString(TEXT("Height:"));
	FText MinesText = FText::FromString(TEXT("Mines:"));

	//Set up fonts
	const FSlateFontInfo& BoldFont = FCoreStyle::GetDefaultFontStyle("Bold", 32);
	const FSlateFontInfo& RegularFont = FCoreStyle::GetDefaultFontStyle("Regular", 32);

	//helper function to build label-input pairs
	//All lambda's params are acquired as & ([&])

	auto CreateLabeledInput = [&](const FText& LabelText, TSharedPtr<SEditableTextBox>& InputBox,
	                              const FString& DefaultValue)
	{
		return SNew(SHorizontalBox)
				//Label Box
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					SNew(STextBlock)
					.Font(BoldFont)
					.Text(LabelText)
				]
				//Input Box
				+ SHorizontalBox::Slot()
				.Padding(InputPadding, 0)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				.AutoWidth()
				[
					SAssignNew(InputBox, SEditableTextBox)
					.Justification(ETextJustify::Left)
					.Font(RegularFont)
					.Text(FText::FromString(DefaultValue))
				];
	};


	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			//Main vertical layout
			SNew(SVerticalBox)
			// Settings Box
			+ SVerticalBox::Slot()
			.Padding(100, 25)
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				//Width Setting
				+ SHorizontalBox::Slot()
				.Padding(LabelPadding, 0)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					//LOCTEXT shows in Editor the text "Width:" and another version called "WidthLabel" usable in this file
					//better understanding what label is created here
					CreateLabeledInput(LOCTEXT("WidthLabel", "Width:"), WidthSetTextBox,
					                   FString::FromInt(DefaultGridWidth))
				]

				//Height Setting
				+ SHorizontalBox::Slot()
				.Padding(LabelPadding, 0)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					CreateLabeledInput(
						LOCTEXT("HeightLabel", "Height:       "), HeightSetTextBox, FString::FromInt(DefaultGridHeight))
				]

				//Mines Setting 
				+ SHorizontalBox::Slot()
				.Padding(LabelPadding, 0)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					CreateLabeledInput(LOCTEXT("MinesLabel", "Mines:"), MinesSetTextBox,
					                   FString::FromInt(DefaultGridMines))
				]
			]

			//New Game Button
			+ SVerticalBox::Slot()
			  .HAlign(HAlign_Center)
			  .VAlign(VAlign_Top)
			  //.FillHeight(1.f)
			  .Padding(20, 20, 20, 0)
			[
				SNew(SButton)
				.ContentPadding(FMargin(50, 10))
				.OnClicked_Raw(this, &FMinesweeperModule::GenerateGrid)
				[
					SNew(STextBlock)
					.Font(BoldFont)
					.Text(LOCTEXT("NewGameButton", "New Game"))

				]
			]

			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.Padding(0, -100, 0, 20)
			[
				SAssignNew(GridPanel, SUniformGridPanel)
			]
			// //Game Over Box
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			.FillHeight(0.1f)
			// // [
			// //
			// // ]


		];
}

FReply FMinesweeperModule::GenerateGrid()
{
	ResetGameState();

	if (!ValidateInputs())
	{
		return FReply::Handled();
	}

	//Create grid
	GridButtons.Reserve(GridHeight * GridWidth);

	for (int32 y = 0; y < GridHeight; ++y)
	{
		for (int32 x = 0; x < GridWidth; ++x)
		{
			SUniformGridPanel::FSlot* Slot;
			GridPanel->AddSlot(x, y).Expose(Slot);
			Slot->SetHorizontalAlignment(HAlign_Fill);
			Slot->SetVerticalAlignment(VAlign_Fill);

			TSharedPtr<SMinesweeperButton> MinesweeperButton;
			Slot->AttachWidget(
				SAssignNew(MinesweeperButton, SMinesweeperButton)
				.bIsMine(false)
				.GridPosition(FVector2D(x, y))
				.MinesweeperModule(this)
			);
			GridButtons.Add(MinesweeperButton);
		}
	}

	PlaceMines();
	return FReply::Handled();
}

bool FMinesweeperModule::ValidateInputs()
{
	if (!HeightSetTextBox || !WidthSetTextBox || !MinesSetTextBox)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid settings not initialized"));
		return false;
	}
	const FText HeightText = HeightSetTextBox->GetText();
	const FText WidthText = WidthSetTextBox->GetText();
	const FText MinesText = MinesSetTextBox->GetText();

	if (!HeightText.IsNumeric() && !WidthText.IsNumeric() && !MinesText.IsNumeric())
	{
		UE_LOG(LogTemp, Error, TEXT("Grid settings not numeric"));
		return false;
	}

	//Parsing and cap input values
	GridHeight = FMath::Min(FCString::Atoi(*HeightText.ToString()), 10);
	GridWidth = FMath::Min(FCString::Atoi(*WidthText.ToString()), 10);
	TotalMines = FMath::Min(FCString::Atoi(*MinesText.ToString()), (GridHeight * GridWidth) - 1);

	return true;
}

void FMinesweeperModule::PlaceMines()
{
	int32 MinesToPlace = TotalMines;
	int32 TotalCells = GridHeight * GridWidth;

	TArray<int32> MineIndices;
	MineIndices.Reserve(TotalCells);

	for (int32 i = 0; i < TotalCells; ++i)
	{
		MineIndices.Add(i);
	}

	//Shuffle array (Fisher-Yates)
	for (int32 i = 0; i < MinesToPlace; ++i)
	{
		int32 j = FMath::RandRange(i, MineIndices.Num() - 1);
		Swap(MineIndices[i], MineIndices[j]);

		int32 MineIndex = MineIndices[i];
		int32 X = MineIndex % GridWidth;
		int32 Y = MineIndex / GridWidth;

		//Get button at this position and set it as mine
		for (TSharedPtr<SMinesweeperButton>& Button : GridButtons)
		{
			FVector2D GridPosition = Button->GetGridPosition();
			if (FMath::IsNearlyEqual(GridPosition.X, X) && FMath::IsNearlyEqual(GridPosition.Y, Y))
			{
				Button->SetIsMine(true);
				break;
			}
		}
	}
}

void FMinesweeperModule::RevealNeighbours(const FVector2D InPosition)
{
	for (TSharedPtr<SMinesweeperButton>& Button : GridButtons)
	{
		if (!Button->IsClicked() && Button->IsNeighbor(InPosition) && !Button->IsMine())
		{
			const int Count = CountNeighborMines(Button->GetGridPosition());

			Button->SetClicked();

			Button->SetNeighborMineCount(Count);
			//Button->SetColorAndOpacity(FLinearColor::Green);
		}
	}
}

int FMinesweeperModule::CountNeighborMines(const FVector2D InPosition)
{
	int Count = 0;

	for (TSharedPtr<SMinesweeperButton>& Button : GridButtons)
	{
		if (!Button->IsClicked() && Button->IsNeighbor(InPosition) && Button->IsMine())
		{
			Count++;
		}
	}

	return Count;
}

void FMinesweeperModule::MineClicked()
{
	for (const TSharedPtr<SMinesweeperButton>& Button : GridButtons)
	{
		Button->RevealMine();
		
	}
}

void FMinesweeperModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
}

void FMinesweeperModule::ResetGameState()
{
	//Clear UI and reset game variables
	GridPanel->ClearChildren();
	GridButtons.Empty();
	GridHeight = 0;
	GridWidth = 0;
	TotalMines = 0;
	bIsFirstTurn = true;
	//GameOverTextBox->SetVisibility(EVisibility::Hidden);
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
