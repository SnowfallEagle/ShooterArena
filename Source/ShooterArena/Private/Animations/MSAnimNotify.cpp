// ShooterArena Game, All Rights Reserved.

#include "Animations/MSAnimNotify.h"

void UMSAnimNotify::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    OnNotified.Broadcast(MeshComponent);
    Super::Notify(MeshComponent, Animation);
}
