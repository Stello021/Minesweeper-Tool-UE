// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minesweeper.h"
#include "Widgets/SCompoundWidget.h"

class FMinesweeperModule;

/**
 * 
 */
class MINESWEEPER_API SMinesweeperButton : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperButton)
		: _bIsMine(false),
		_GridPosition(0, 0)
	{}
		SLATE_ARGUMENT(bool, bIsMine)
		SLATE_ARGUMENT(FVector2D, GridPosition)
		SLATE_ARGUMENT(FMinesweeperModule*, MinesweeperModule)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	void SetMine(bool const bSetMine);
	void RevealMine() const;

	void RevealNeighbours() const;
	void SetNeighborMineCount(int const Count) const;

	bool IsNeighbor(const FVector2D InPosition) const;
	bool IsClicked() const { return bIsClicked; }
	bool IsMine() const { return bIsMine; }

	void SetClicked();
	FReply HandleClick();

	FVector2D GetGridPosition() const;
	void SetIsMine(bool const bSetMine);

private:
	uint8 bIsMine : 1;
	uint8 bIsClicked : 1;

	FVector2D GridPosition {};
	
	FMinesweeperModule* MinesweeperModule;

	TSharedPtr<SButton> Button;
	TSharedPtr<STextBlock> ButtonText;
};
