// ShooterArena Game, All Rights Reserved.

#include "Sound/MSSoundLibrary.h"
#include "Sound/SoundClass.h"

DEFINE_LOG_CATEGORY_STATIC(LogMSSoundLibrary, All, All);

void UMSSoundLibrary::SetSoundClassVolume(USoundClass* Class, float Volume)
{
    if (Class)
    {
        Class->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
        UE_LOG(LogMSSoundLibrary, Display, TEXT("SoundClass %s volume has changed to %.2f"), *Class->GetName(), Class->Properties.Volume)
    }
}

void UMSSoundLibrary::ToggleSoundClassVolume(USoundClass* Class)
{
    if (Class)
    {
        SetSoundClassVolume(Class, Class->Properties.Volume > 0.0f ? 0.0f : 1.0f);
    }
}
