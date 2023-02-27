#pragma once

#include "GameFramework/Actor.h"

class FCoreUtils
{
public:
    template<typename T>
    static T* GetActorComponent(AActor* Actor)
    {
        if (!Actor)
        {
            return nullptr;
        }

        const auto Component = Actor->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }
};
