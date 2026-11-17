// CSpawnPointComponent's unit (2704): empties the component's voActorBlueprint (reflection's clear of a
// CRntVector<CSpawnerActorBlueprint>), destroying each element (its payload is released through the game
// manager or deleted) and setting the count to zero.
#include "base/global/CRntVector.h"
#include "game/CSpawnerActorBlueprint.h"
#include "types.h"

void fn_710112f85c(base::global::CRntVector<CSpawnerActorBlueprint>* self) {
    self->Clear();
}
