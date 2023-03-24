// MyShooter Game, All Rights Reserved.

#include "Menu/UI/MSMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMSMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UMSMenuWidget::OnStart);
    }
}

void UMSMenuWidget::OnStart()
{
    UGameplayStatics::OpenLevel(this, "TestLevel");
}
