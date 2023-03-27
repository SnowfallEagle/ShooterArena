// MyShooter Game, All Rights Reserved.

#include "AI/MSAICharacter.h"
#include "AI/MSAIController.h"
#include "BrainComponent.h"
#include "Components/MSAIWeaponComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/MSHealthComponent.h"
#include "UI/MSHealthBarWidget.h"

AMSAICharacter::AMSAICharacter(const FObjectInitializer& ObjInit) :
    Super(ObjInit.SetDefaultSubobjectClass<UMSAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = AMSAIController::StaticClass();

    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);

    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBarWidgetComponent");
    check(HealthBarWidgetComponent);
    HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
    HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidgetComponent->SetDrawAtDesiredSize(true);

    SquaredHealthBarDrawDistance = HealthBarDrawDistance * HealthBarDrawDistance;
}

void AMSAICharacter::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
        CheckHealthBarTimer, this, &AMSAICharacter::OnCheckHealthBarVisibility, CheckHealthBarVisibilityTimeRate, true
    );
}

void AMSAICharacter::EndPlay(EEndPlayReason::Type Reason)
{
    Super::EndPlay(Reason);

    GetWorldTimerManager().ClearTimer(CheckHealthBarTimer);
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

void AMSAICharacter::OnHealthChanged(float NewHealth, float HealthDelta)
{
    Super::OnHealthChanged(NewHealth, HealthDelta);

    if (const auto HealthBarWidget = Cast<UMSHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject()))
    {
        HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
    }
}

void AMSAICharacter::OnCheckHealthBarVisibility()
{
    const APawn* PlayerPawn = nullptr;
    const UWorld* World = GetWorld();

    if (World)
    {
        if (const APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            PlayerPawn = PlayerController->GetPawnOrSpectator();
        }
    }

    if (!PlayerPawn)
    {
        return;
    }

    if (const auto HealthBarWidget = Cast<UMSHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject()))
    {
        const FVector PlayerLocation = PlayerPawn->GetActorLocation();
        const FVector MyLocation = GetActorLocation();
        const bool bDistanceVisibility = FVector::DistSquared(PlayerLocation, MyLocation) < SquaredHealthBarDrawDistance;

        const FVector MeToPlayer = (PlayerLocation - MyLocation).GetSafeNormal();
        const FVector EndLocation = MyLocation + MeToPlayer * HealthBarDrawDistance;

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this);

        const bool bTraced = World->LineTraceSingleByChannel( //
            HitResult, MyLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParams
        );
        const bool bTraceVisibility = bTraced && HitResult.Actor.Get() == PlayerPawn;

        HealthBarWidget->SetVisibility(bDistanceVisibility && bTraceVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}
