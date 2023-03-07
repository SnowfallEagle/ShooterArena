// MyShooter Game, All Rights Reserved.

#include "AI/EQS/EnvQueryTest_PickupCanBeTaken.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/MSPickup.h"

UEnvQueryTest_PickupCanBeTaken::UEnvQueryTest_PickupCanBeTaken(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCanBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    bool bWantToBeTakable = BoolValue.GetValue();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AMSPickup* Pickup = Cast<AMSPickup>(GetItemActor(QueryInstance, It.GetIndex()));
        if (!Pickup)
        {
            continue;
        }

        It.SetScore(TestPurpose, FilterType, Pickup->CanBeTaken(), bWantToBeTakable);
    }
}
