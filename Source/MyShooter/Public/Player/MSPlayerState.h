// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MSPlayerState.generated.h"

UCLASS()
class MYSHOOTER_API AMSPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    int32 GetTeamID() const { return TeamID; }
    void SetTeamID(int32 ID) { TeamID = ID; }

    FLinearColor GetTeamColor() const { return TeamColor; }
    void SetTeamColor(const FLinearColor& Color) { TeamColor = Color; }

private:
    int32 TeamID;
    FLinearColor TeamColor;
};
