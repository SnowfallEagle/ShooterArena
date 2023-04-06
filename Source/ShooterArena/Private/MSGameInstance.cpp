// ShooterArena Game, All Rights Reserved.

#include "MSGameInstance.h"
#include "Sound/MSSoundLibrary.h"

void UMSGameInstance::ToggleVolume()
{
    UMSSoundLibrary::ToggleSoundClassVolume(MasterSoundClass);
}
