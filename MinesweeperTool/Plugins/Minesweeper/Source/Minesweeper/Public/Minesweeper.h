// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMinesweeperModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TSharedPtr<SEditableTextBox> WidthSetTextBox;
	TSharedPtr<SEditableTextBox> HeightSetTextBox;
	TSharedPtr<SEditableTextBox> MinesSetTextBox;

	// `{}` ensures the variables are initialized with 0 rather
	// than containing random memory garbage
	int GridHeight {};
	int GridWidth {};
	int TotalMines {};
};
