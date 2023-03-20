// MyShooter Game, All Rights Reserved.

#include "AI/MSAICharacter.h"
#include "AI/MSAIController.h"
#include "Components/MSAIWeaponComponent.h"
#include "BrainComponent.h"

AMSAICharacter::AMSAICharacter(const FObjectInitializer& ObjInit) :
    Super(ObjInit.SetDefaultSubobjectClass<UMSAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = AMSAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}

void AMSAICharacter::OnDeath()
{
    Super::OnDeath();

    const auto* MSController = GetController<AMSAIController>();
    if (MSController && MSController->BrainComponent)
    {
        MSController->BrainComponent->Cleanup();
    }
}
