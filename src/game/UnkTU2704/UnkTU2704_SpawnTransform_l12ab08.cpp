// UnkTU2704 (CSpawnPointComponent's unit): where the spawn point spawns. The owner's position and angles,
// unless the dynamic spawn position is on: then those of the object found earlier (+0x380), or else of the
// link chosen by eDynamicSpawnPositionMode: nearest to the player, farthest from it, or one at random.
// Names and layouts are placeholders.
#include "game/CSpawnPointComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" int rand();

static inline UnkVec3f Position(UnkActor* actor) {
    return UnkVec3f(actor->field_0x44, actor->field_0x54, actor->field_0x64);
}

static inline UnkVec3f Angle(UnkActor* actor) {
    return reinterpret_cast<UnkXform*>(actor->field_0x38)->GetAngle();
}

static inline f32 Square(f32 v) {
    return v * v;
}

static inline f32 DistanceSq(UnkActor* a, UnkActor* b) {
    return Square(a->field_0x44 - b->field_0x44) + Square(a->field_0x54 - b->field_0x54) +
           Square(a->field_0x64 - b->field_0x64);
}

// The link's target, resolved first.
static inline UnkActor* Target(CGameLink<CActor>& link) {
    CSpawnPointComponent::ResolveLink(link);
    return reinterpret_cast<UnkActor*>(link.Target());
}

// The object found by fn_710112c4d8, or null.
static inline UnkActor* Found(CSpawnPointComponent* self) {
    UnkWeakHandle* handle = self->field_0x380;
    return reinterpret_cast<UnkActor*>(handle ? handle->target : nullptr);
}

// The link target nearest to (or farthest from) the player, starting from the owner.
static inline UnkActor* ByDistance(CSpawnPointComponent* self, UnkActor* player, bool farthest) {
    UnkActor* best = self->Owner<UnkActor>();
    f32 best_distance = DistanceSq(player, best);
    for (CGameLink<CActor>& link : self->tDynamicSpawnPositions) {
        UnkActor* target = Target(link);
        if (!target)
            continue;
        f32 distance = DistanceSq(player, target);
        if (!best || (farthest ? best_distance < distance : best_distance > distance)) {
            best_distance = distance;
            best = target;
        }
    }
    return best;
}

void fn_710112ab08(CSpawnPointComponent* self, UnkVec3f* pos, UnkVec3f* angle) {
    *pos = Position(self->Owner<UnkActor>());
    *angle = Angle(self->Owner<UnkActor>());
    if (!self->bUseDynamicSpawnPosition)
        return;
    if (UnkActor* found = Found(self)) {
        *pos = Position(found);
        *angle = Angle(Found(self));
        return;
    }
    u32 count = self->tDynamicSpawnPositions.count;
    if (count == 0)
        return;
    switch (static_cast<s32>(self->eDynamicSpawnPositionMode)) {
    case 0: {
        UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
        if (!player)
            return;
        UnkActor* best = ByDistance(self, player, false);
        *pos = Position(best);
        *angle = Angle(best);
        return;
    }
    case 1: {
        UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
        if (!player)
            return;
        UnkActor* best = ByDistance(self, player, true);
        *pos = Position(best);
        *angle = Angle(best);
        return;
    }
    case 2: {
        u32 i = (rand() & 0x7fff) % count;
        if (!Target(self->tDynamicSpawnPositions.data[i]))
            return;
        *pos = Position(Target(self->tDynamicSpawnPositions.data[i]));
        *angle = Angle(Target(self->tDynamicSpawnPositions.data[i]));
        return;
    }
    default:
        return;
    }
}
