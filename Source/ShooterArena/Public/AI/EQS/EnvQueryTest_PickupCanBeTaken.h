// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickupCanBeTaken.generated.h"

UCLASS()
class SHOOTERARENA_API UEnvQueryTest_PickupCanBeTaken : public UEnvQueryTest
{
    GENERATED_BODY()

public:
    UEnvQueryTest_PickupCanBeTaken(const FObjectInitializer& ObjInit);

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
