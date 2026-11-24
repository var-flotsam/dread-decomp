// behaviortree::CScourgeComputeAttackPositionAction's unit: runs the node. It takes the owner's position (or
// the movement's, when its movement answers 1), asks the Scourge AI component for a position, and keeps the
// previous one while the new one stays within 5 units of it; the answers go to the named blackboard values.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

void* fn_71000839a0(u64 size, u32 flags);
bool fn_71006a1238(void* state, UnkActor* actor, bool* moving, CVector3D* pos, u8* mode);
bool fn_71006a249c(void* list, UnkActor* actor, bool flag);
void fn_7100395600(UnkSelf* behavior, void* record, const CVector3D* value);

struct UnkScourgeAI_l69d8c4 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x14750 - 0x8];
    u8 field_0x14750[1];
};
struct UnkScourgeAI_sca;
struct CScourgeAIComponent;
bool fn_71006a07e0(UnkScourgeAI_sca* self);
bool fn_71006a0884(CScourgeAIComponent* self);

struct UnkMovement_l69d8c4 {
    u8 pad_0x0[0x8c];
    CVector2D xy;  // 0x8c
};

struct UnkOwnerComponent_l69d8c4 {
    u8 pad_0x0[0x8e8];
    UnkMovement_l69d8c4 movement;  // 0x8e8
    u8 pad_0x8e8_end[0x1c08 - 0x8e8 - sizeof(UnkMovement_l69d8c4)];
    u8 mode;  // 0x1c08
};

struct UnkScourgeAttackPos_l69d8c4 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x10];
    base::global::CStrId position_key;  // 0x30
    base::global::CStrId mode_key;      // 0x38
    base::global::CStrId moved_key;     // 0x40
    CVector3D last;                     // 0x48
};

static inline UnkMovement_l69d8c4* GetMovement(UnkOwnerComponent_l69d8c4* comp) {
    return fn_71010cc3a4(comp) == 1 ? &comp->movement : nullptr;
}

static inline bool Near(const CVector3D& a, const CVector3D& b) {
    return !(a.x + 5.0f < b.x) && !(a.x - 5.0f > b.x) && !(a.y + 5.0f < b.y) && !(a.y - 5.0f > b.y) &&
           !(a.z + 5.0f < b.z) && !(a.z - 5.0f > b.z);
}

static inline void SetIfNamed(UnkScourgeAttackPos_l69d8c4* self, base::global::CStrId& key, bool value) {
    if (key.data != &g_unk_7101d47108->empty)
        fn_7100395474(reinterpret_cast<UnkSelf*>(self), &key, value);
}

s32 fn_710069d8c4(UnkScourgeAttackPos_l69d8c4* self) {
    UnkScourgeAI_l69d8c4* ai = nullptr;
    if (self->owner) {
        ai = static_cast<UnkScourgeAI_l69d8c4*>(self->owner->field_0x240);
        if (ai && !fn_710009d50c(ai->vf_0x0(), fn_710069ef44()))
            ai = nullptr;
    }
    UnkOwnerComponent_l69d8c4* comp = nullptr;
    if (self->owner)
        comp = static_cast<UnkOwnerComponent_l69d8c4*>(self->owner->field_0x218);
    bool moving = false;
    CVector3D start;
    start.x = self->owner->field_0x44;
    start.y = self->owner->field_0x54;
    start.z = self->owner->field_0x64;
    if (comp && fn_71010cc3a4(comp) == 1) {
        CVector2D xy = GetMovement(comp)->xy;
        start.z = 0;
        start.x = xy.x;
        start.y = xy.y;
    }
    CVector3D pos = start;
    u8 mode = comp->mode;
    if (!ai)
        return 6;
    if (!fn_71006a1238(ai->field_0x14750, self->owner, &moving, &pos, &mode))
        return 6;
    bool result;
    bool settled = true;
    if (fn_71006a07e0(reinterpret_cast<UnkScourgeAI_sca*>(ai)) &&
        fn_71006a0884(reinterpret_cast<CScourgeAIComponent*>(ai))) {
        UnkManager_71726af468* manager = g_unk_71726af468;
        if (!manager) {
            manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
            fn_7100da7e2c(manager);
            g_unk_71726af468 = manager;
        }
        if (!fn_71006a249c(manager->field_0x5e0, self->owner, true)) {
            settled = false;
            result = true;
            moving = true;
            pos = start;
        }
    }
    if (settled) {
        if (!moving) {
            result = false;
        } else if (Near(pos, start) || Near(pos, self->last)) {
            result = false;
            moving = false;
            self->last = pos;
        } else {
            result = true;
        }
    }
    SetIfNamed(self, self->moved_key, result);
    SetIfNamed(self, self->mode_key, mode);
    if (self->position_key.data != &g_unk_7101d47108->empty)
        fn_7100395600(reinterpret_cast<UnkSelf*>(self), &self->position_key, &pos);
    return 2;
}
