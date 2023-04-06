// ShooterArena Game, All Rights Reserved.

#include "Components/MSCharacterMovementComponent.h"
#include "Character/MSCharacter.h"

float UMSCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const AMSCharacter* Character = Cast<AMSCharacter>(GetPawnOwner());

    return Character && Character->IsRunning() ? MaxSpeed * RunSpeedModifier : MaxSpeed;
}
