// MyShooter Game, All Rights Reserved.

#include "UI/MSHealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMSHealthBarWidget::SetHealthPercent(float Health)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(Health);
        HealthBar->SetFillColorAndOpacity(Health > ColorThreshold ? HealthyColor : DyingColor);
        HealthBar->SetVisibility(Health > 0.0f && Health < VisibilityThreshold ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}
