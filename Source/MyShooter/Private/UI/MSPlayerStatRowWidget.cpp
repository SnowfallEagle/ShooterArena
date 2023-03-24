// MyShooter Game, All Rights Reserved.

#include "UI/MSPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UMSPlayerStatRowWidget::SetName(const FText& Name)
{
    if (NameTextBlock)
    {
        NameTextBlock->SetText(Name);
    }
}

void UMSPlayerStatRowWidget::SetKills(const FText& Name)
{
    if (KillsTextBlock)
    {
        KillsTextBlock->SetText(Name);
    }
}

void UMSPlayerStatRowWidget::SetDeaths(const FText& Name)
{
    if (DeathsTextBlock)
    {
        DeathsTextBlock->SetText(Name);
    }
}

void UMSPlayerStatRowWidget::SetTeam(const FText& Name)
{
    if (TeamTextBlock)
    {
        TeamTextBlock->SetText(Name);
    }
}

void UMSPlayerStatRowWidget::TogglePlayerIndicatorVisibility(bool bVisible)
{
    if (PlayerIndicatorImage)
    {
        PlayerIndicatorImage->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}
