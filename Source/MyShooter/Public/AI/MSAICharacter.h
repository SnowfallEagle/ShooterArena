// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/MSCharacter.h"
#include "MSAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class MYSHOOTER_API AMSAICharacter : public AMSCharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTree;

public:
    AMSAICharacter(const FObjectInitializer& ObjInit);
};
