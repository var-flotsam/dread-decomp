// UnkTU2704 (CSpawnPointComponent's unit): the spawn point component's destructor. It drops the two
// colliders and what their shapes are tracked as, then destroys the members last first and ends with the
// base component's destructor.
#include "base/UnkCountedResource.h"
#include "game/CSpawnPointComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU2704.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// Drops a counted reference to a weak handle.
static inline void Release(UnkWeakHandle* handle) {
    if (handle && --handle->ref_count == 0)
        handle->vf_0x10();
}

void fn_7101125c90(char* p) {
    CSpawnPointComponent* self = reinterpret_cast<CSpawnPointComponent*>(p);
    self->vtable = g_unk_7101cb28c8 + 2;
    fn_71011264b4(self);
    fn_71011265c0(self);
    fn_71011266cc(&self->voActorBlueprint);
    Release(self->field_0x380);
    self->tDynamicSpawnPositions.Free();
    Release(self->field_0x348);
    Release(self->field_0x340);
    self->wpXCellActivationAreaShape.~CGameLink();
    self->tXCellTransformTargets.Free();
    self->field_0x290.~UnkOwnedStrIdTable();
    self->field_0x288.~CStrId();
    self->wpAreaOfInterestEnd.~CGameLink();
    self->wpAreaOfInterest.~CGameLink();
    self->wpNavigableShape.~CGameLink();
    self->wpCollisionSceneShapeId.~CGameLink();
    self->wpSceneShapeId.~CGameLink();
    self->sOnEntityGenerated.~CRntString();
    self->sOnBeforeGenerate.~CRntString();
    self->sStartAnimation.~CStrId();
    self->field_0x60.~CGameLink();
    self->field_0x58.~CStrId();
    if (UnkCountedResource* resource = self->field_0x50) {
        if (--resource->ref_count == 0)
            fn_710001737c(resource);
    }
    self->field_0x48.~CStrId();
    self->field_0x40.~UnkRefPtrMember();
    fn_71010c4194(self);
}
