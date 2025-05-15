// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperButton.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SMinesweeperButton::Construct(const FArguments& InArgs)
{
	bIsMine = InArgs._bIsMine;
	MinesweeperModule = InArgs._MinesweeperModule;
	GridPosition = InArgs._GridPosition;
	bIsClicked = false;

	constexpr float ButtonSize = 50.f;

	//Widget Hierarchy
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(ButtonSize)
		.HeightOverride(ButtonSize)

		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SAssignNew(Button, SButton)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.OnClicked(this, &SMinesweeperButton::HandleClick)

			]

			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(ButtonText, STextBlock)
				.Visibility(EVisibility::Hidden)
				.ColorAndOpacity(bIsMine ? FLinearColor::Red : FCoreStyle::Get().GetSlateColor("Button.Default"))
				.Text(bIsMine ? FText::FromString("X") : FText::GetEmpty())
			]
		]
	];

	SetVisibility(EVisibility::SelfHitTestInvisible);
}

void SMinesweeperButton::SetMine(bool const bSetMine)
{
	bIsMine = bSetMine;
	//Cache the text objects to avoid recreating them
	static const FText MineText = FText::FromString("X");
	static const FText EmptyText = FText::GetEmpty();
	ButtonText->SetText(bIsMine ? MineText : EmptyText);
}


void SMinesweeperButton::RevealMine() const
{
	if (bIsMine)
	{
		ButtonText->SetText(FText::FromString("X"));
		ButtonText->SetColorAndOpacity(FLinearColor::Red);
		ButtonText->SetVisibility(EVisibility::HitTestInvisible);
	}
}

void SMinesweeperButton::RevealNeighbours() const
{
	if (MinesweeperModule)
	{
		MinesweeperModule->RevealNeighbours(GridPosition);
	}
}

void SMinesweeperButton::SetNeighborMineCount(int const Count) const
{
	//Cache number text to avoid recreating it
	static TMap<int32, FText> NumberCache;

	//Get or create cached text
	const FText* CachedText = NumberCache.Find(Count);
	if (!CachedText)
	{
		NumberCache.Add(Count, FText::AsNumber(Count));
		CachedText = NumberCache.Find(Count);
	}

	ButtonText->SetText(*CachedText);
	ButtonText->SetColorAndOpacity(FLinearColor::Green);
	ButtonText->SetVisibility(EVisibility::HitTestInvisible);
}

bool SMinesweeperButton::IsNeighbor(const FVector2D InPosition) const
{
	// Calculate the difference in X and Y coordinates
	int32 DeltaX = FMath::Abs(GridPosition.X - InPosition.X);
	int32 DeltaY = FMath::Abs(GridPosition.Y - InPosition.Y);

	// Check if the coordinates are adjacent (1 unit away in any direction)
	return (DeltaX <= 1 && DeltaY <= 1) && !(DeltaX == 0 && DeltaY == 0);
}

void SMinesweeperButton::SetClicked()
{
	Button->SetEnabled(false);
	bIsClicked = true;
}

FReply SMinesweeperButton::HandleClick()
{
	SetClicked();

	if (!MinesweeperModule)
	{
		return FReply::Handled();
	}

	//Handle first turn special case
	// if (MinesweeperModule->IsFirstTurn())
	// {
	// 	bIsMine = false;
	// 	MinesweeperModule->MoveMineOnFirstTurn();
	// }

	//Handle mine case
	if (bIsMine)
	{
		RevealMine();
		MinesweeperModule->MineClicked();
		return FReply::Handled();
	}

	//Handle regular case
	const int32 Count = MinesweeperModule->CountNeighborMines(GridPosition);
	if (Count == 0)
	{
		RevealNeighbours();
	}

	SetNeighborMineCount(Count);

	return FReply::Handled();
}

FVector2D SMinesweeperButton::GetGridPosition() const
{
	return GridPosition;
}

void SMinesweeperButton::SetIsMine(bool const bSetMine)
{
	bIsMine = bSetMine;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
