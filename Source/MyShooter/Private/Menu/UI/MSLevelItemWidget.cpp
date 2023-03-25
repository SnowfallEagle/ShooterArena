// MyShooter Game, All Rights Reserved.

#include "Menu/UI/MSLevelItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UMSLevelItemWidget::NativeOnInitialized()
{
    if (LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &UMSLevelItemWidget::OnLevelSelect);
    }

    ToggleLevelSelected(false);
}

void UMSLevelItemWidget::SetLevelData(const FLevelData& InLevelData)
{
    LevelData = InLevelData;

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(LevelData.DisplayName));
    }

    if (LevelImage)
    {
        LevelImage->SetBrushFromTexture(LevelData.LevelImage);
    }

    if (FrameImage)
    {
        FrameImage->SetBrushFromTexture(LevelData.FrameImage);
    }
}

void UMSLevelItemWidget::ToggleLevelSelected(bool bSelected)
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UMSLevelItemWidget::OnLevelSelect()
{
    OnLevelSelected.Broadcast(LevelData);
}
