// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MSGameModeBase.generated.h"

class AAIController;

UCLASS()
class MYSHOOTER_API AMSGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1"))
    int32 NumPlayers = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1"))
    int32 NumRounds = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1"))
    int32 RoundTime = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team")
    FLinearColor DefaultTeamColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team")
    TArray<FLinearColor> TeamColors;

private:
    int32 CurrentRound;
    int32 RoundTimeLeft;
    FTimerHandle RoundTimer;

public:
    AMSGameModeBase();

    virtual void StartPlay() override;
    UClass* GetDefaultPawnClassForController_Implementation(AController* InController);

private:
    void SpawnBots();
    void StartRound();
    void ResetPlayers();

    void OnRoundUpdate();
};
