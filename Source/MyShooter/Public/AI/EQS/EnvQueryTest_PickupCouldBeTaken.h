// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickupCouldBeTaken.generated.h"

UCLASS()
class MYSHOOTER_API UEnvQueryTest_PickupCouldBeTaken : public UEnvQueryTest
{
    GENERATED_BODY()

public:
    UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjInit);

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
