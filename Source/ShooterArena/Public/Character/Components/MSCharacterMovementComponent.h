// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MSCharacterMovementComponent.generated.h"

UCLASS()
class SHOOTERARENA_API UMSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 1.5, ClampMax = 10.0))
    float RunSpeedModifier = 2.0f;
	
public:
	virtual float GetMaxSpeed() const override;
};
