// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MSPlayerState.generated.h"

UCLASS()
class MYSHOOTER_API AMSPlayerState : public APlayerState
{
    GENERATED_BODY()

private:
    int32 TeamID;
    FLinearColor TeamColor;

    int32 Kills = 0;
    int32 Deaths = 0;

public:
    FORCEINLINE int32 GetTeamID() const { return TeamID; }
    FORCEINLINE void SetTeamID(int32 ID) { TeamID = ID; }

    FORCEINLINE const FLinearColor& GetTeamColor() const { return TeamColor; }
    FORCEINLINE void SetTeamColor(const FLinearColor& Color) { TeamColor = Color; }

    UFUNCTION(BlueprintCallable, Category = "Stat")
    FORCEINLINE int32 GetKills() const { return Kills; }

    FORCEINLINE void AddKill() { ++Kills; }

    UFUNCTION(BlueprintCallable, Category = "Stat")
    FORCEINLINE int32 GetDeaths() const { return Deaths; }

    FORCEINLINE void AddDeath() { ++Deaths; }

    void LogInfo();
};
