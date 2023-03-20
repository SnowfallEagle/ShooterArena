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
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player", meta = (ClampMin = "1"))
    int32 NumPlayers = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player", meta = (ClampMin = "0", ClampMax = "20"))
    int32 RespawnTime = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player", meta = (ClampMin = "0"))
    int32 MinRoundTimeToSpawn = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Round", meta = (ClampMin = "1"))
    int32 NumRounds = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Round", meta = (ClampMin = "1"))
    int32 RoundTime = 10;

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

    void ReportKill(AController* Killer, AController* Victim);
    void RequestSpawn(AController* Controller);

    UFUNCTION(BlueprintCallable, Category = "Round")
    FORCEINLINE int32 GetCurrentRound() const { return FMath::Min(CurrentRound, NumRounds); }

    UFUNCTION(BlueprintCallable, Category = "Round")
    FORCEINLINE int32 GetNumRounds() const { return NumRounds; }

    UFUNCTION(BlueprintCallable, Category = "Round")
    FORCEINLINE int32 GetRoundTimeLeft() const { return RoundTimeLeft; }

private:
    void SpawnBots();
    void SetTeamInfo();
    void SetCharacterColor(AController* Controller);

    void StartRound();
    void OnRoundUpdate();

    void StartSpawn(AController* Controller);
    void ResetPlayer(AController* Controller);
    void ResetPlayers();
    void LogPlayerStates();

    void EndGame();
};
