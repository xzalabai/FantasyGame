#include "Fist.h"
#include "Components/BoxComponent.h"

void AFist::BeginPlay()
{
    Super::BeginPlay();
    ItemState = EItemState::EIS_Equipped;
}

