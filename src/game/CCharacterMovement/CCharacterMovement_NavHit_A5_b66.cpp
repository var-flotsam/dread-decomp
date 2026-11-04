// CCharacterMovement's unit: the navigation hit at the owner's position (virtual 0x2e0, from the
// actor's position at +0x44/+0x54/+0x64), or an empty hit when virtual 0x2d0 gives no target.
// UnkNavMeshHit as in src/game/navmesh/CNavMesh.cpp. Names are placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkNavMeshHit;
struct UnkElem_7100286624;  // the same 12-byte record (src/game_lo/fn_7100286624.cpp)
void fn_7100287598(UnkNavMeshHit* self);
void fn_710028760c(UnkElem_7100286624* self);

struct UnkNavMeshHit {
    u16 cell;
    u8 side;
    CVector2D point;

    UnkNavMeshHit() { fn_7100287598(this); }
    UnkNavMeshHit(const UnkNavMeshHit& other) {
        __builtin_memcpy(static_cast<void*>(&cell), &other.cell, 4);  // the cell, the side and the pad
        point.x = other.point.x;
        point.y = other.point.y;
    }
    ~UnkNavMeshHit() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }

    bool IsValid() const { return cell != 0xffff && side != 0xff; }
};
static_assert(sizeof(UnkNavMeshHit) == 0xc, "UnkNavMeshHit layout");

struct UnkNavPosition_A5_b66 {
    f32 x, y, z;
};

__attribute__((always_inline)) static inline UnkNavPosition_A5_b66 Position_A5_b66(UnkActor* actor) {
    f32 x = actor->field_0x44;
    f32 y = actor->field_0x54;
    f32 z = actor->field_0x64;
    UnkNavPosition_A5_b66 pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    return pos;
}

struct UnkCharacterMovementNav_A5_b66 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30

    void* vf_0x2d0() { return reinterpret_cast<void* (*)(UnkCharacterMovementNav_A5_b66*)>(vtable[0x2d0 / 8])(this); }
    UnkNavMeshHit vf_0x2e0(const UnkNavPosition_A5_b66& pos, bool flag) {
        return reinterpret_cast<UnkNavMeshHit (*)(UnkCharacterMovementNav_A5_b66*, const UnkNavPosition_A5_b66*, bool)>(
            vtable[0x2e0 / 8])(this, &pos, flag);
    }
};

UnkNavMeshHit fn_7100979c3c(UnkCharacterMovementNav_A5_b66* self) {
    UnkNavMeshHit hit;
    if (self->vf_0x2d0()) {
        UnkNavMeshHit found = self->vf_0x2e0(Position_A5_b66(self->owner), true);
        hit.cell = found.cell;
        hit.side = found.side;
        hit.point.x = found.point.x;
        hit.point.y = found.point.y;
    }
    return hit;
}
