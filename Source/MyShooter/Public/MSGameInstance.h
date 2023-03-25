// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MSGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName DisplayName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    UTexture2D* LevelImage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    UTexture2D* FrameImage = nullptr;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);

UCLASS()
class MYSHOOTER_API UMSGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName MainMenuLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ToolTip = "Level names must be unique"))
    TArray<FLevelData> LevelsData;

private:
    FLevelData StartupLevel;

public:
    FORCEINLINE const TArray<FLevelData>& GetLevelsData() const { return LevelsData; }

    FORCEINLINE const FLevelData& GetStartupLevel() const { return StartupLevel; }
    FORCEINLINE void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

    FORCEINLINE FName GetMainMenuLevelName() const { return MainMenuLevelName; }
};
