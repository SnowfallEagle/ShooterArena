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

public:
    void SetName(const FText& Name);
    void SetKills(const FText& Name);
    void SetDeaths(const FText& Name);
    void SetTeam(const FText& Name);
    void TogglePlayerIndicatorVisibility(bool bVisible);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* NameTextBlock;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* KillsTextBlock;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeathsTextBlock;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TeamTextBlock;

    UPROPERTY(meta = (BindWidget))
    UImage* PlayerIndicatorImage;
};
