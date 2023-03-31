#pragma once

#include "GameFramework/Actor.h"
#include "Player/MSPlayerState.h"

class FCoreUtils
{
public:
    template<typename T>
    static T* GetActorComponent(const AActor* Actor)
    {
        if (!Actor)
        {
            return nullptr;
        }

        const auto Component = Actor->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }

    static bool AreEnemies(const AController* First, const AController* Second)
    {
        if (!First || !Second || First == Second)
        {
            return false;
        }

        const AMSPlayerState* FirstPlayerState = First->GetPlayerState<AMSPlayerState>();
        const AMSPlayerState* SecondPlayerState = Second->GetPlayerState<AMSPlayerState>();

        if (!FirstPlayerState || !SecondPlayerState || FirstPlayerState->GetTeamID() == SecondPlayerState->GetTeamID())
        {
            return false;
        }

        return true;
    }

    FORCEINLINE static FText IntToText(int Num) { return FText::FromString(FString::FromInt(Num)); }
};
