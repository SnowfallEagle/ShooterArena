// MyShooter Game, All Rights Reserved.

#include "Animations/MSEquipFinishedAnimNotify.h"

void UMSEquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComponent);
    Super::Notify(MeshComponent, Animation);
}
