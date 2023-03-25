// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSPlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class MYSHOOTER_API UMSPlayerStatRowWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* NameTextBlock = nullptr;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* KillsTextBlock = nullptr;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeathsTextBlock = nullptr;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TeamTextBlock = nullptr;

    UPROPERTY(meta = (BindWidget))
    UImage* PlayerIndicatorImage = nullptr;

public:
    void SetName(const FText& Name);
    void SetKills(const FText& Name);
    void SetDeaths(const FText& Name);
    void SetTeam(const FText& Name);
    void TogglePlayerIndicatorVisibility(bool bVisible);
};
