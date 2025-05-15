// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class SUniformGridPanel;
class SMinesweeperButton;

class FMinesweeperModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	void ResetGameState();
	void RevealNeighbours(const FVector2D InPosition);
	int CountNeighborMines(const FVector2D InPosition);
	void MineClicked();

private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	FReply GenerateGrid();
	bool ValidateInputs();
	void PlaceMines();
	
	
	TSharedPtr<class FUICommandList> PluginCommands;

	TSharedPtr<SEditableTextBox> WidthSetTextBox;
	TSharedPtr<SEditableTextBox> HeightSetTextBox;
	TSharedPtr<SEditableTextBox> MinesSetTextBox;
	TSharedPtr<SUniformGridPanel> GridPanel;
	TSharedPtr<SEditableTextBox> GameOverTextBox;

	TArray<TSharedPtr<SMinesweeperButton>> GridButtons;
	
	

	// `{}` ensures the variables are initialized with 0 rather
	// than containing random memory garbage
	int32 GridHeight {};
	int32 GridWidth {};
	int32 TotalMines {};
	bool bIsFirstTurn {};
};
