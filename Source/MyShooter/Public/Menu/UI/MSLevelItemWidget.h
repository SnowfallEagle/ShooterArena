// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSGameInstance.h"
#include "MSLevelItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class MYSHOOTER_API UMSLevelItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    FOnLevelSelectedSignature OnLevelSelected;

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* LevelSelectButton = nullptr;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelNameTextBlock = nullptr;

    UPROPERTY(meta = (BindWidget))
    UImage* LevelImage = nullptr;

    UPROPERTY(meta = (BindWidget))
    UImage* FrameImage = nullptr;

private:
    FLevelData LevelData;

public:
    const FLevelData& GetLevelData() const { return LevelData; }
    void SetLevelData(const FLevelData& InLevelData);

    void ToggleLevelSelected(bool bSelected);

protected:
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnLevelSelect();
};
