// MyShooter Game, All Rights Reserved.

#include "UI/MSPlayerNameWidget.h"
#include "Components/TextBlock.h"

void UMSPlayerNameWidget::SetName(const FString& Name)
{
    NameTextBlock->SetText(FText::FromString(Name));
}

void UMSPlayerNameWidget::NativeOnInitialized()
{
    NameTextBlock->SetText(FText::FromString(""));
}
