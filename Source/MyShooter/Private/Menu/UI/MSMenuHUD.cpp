// MyShooter Game, All Rights Reserved.

#include "Menu/UI/MSMenuHUD.h"
#include "Menu/UI/MSMenuWidget.h"

void AMSMenuHUD::BeginPlay()
{
    const auto MenuWidget = CreateWidget<UMSMenuWidget>(GetWorld(), MenuWidgetClass);
    if (MenuWidget)
    {
        MenuWidget->AddToViewport();
    }
}
