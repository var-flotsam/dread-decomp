// animtree::CLookAtEntityControl unit (tools/tu_map.py): the control's update. Looks up the entity
// its definition names (fn_7100deba8c), aims at that entity's position (the origin when there is
// none; fn_71002e6794) and then steps (fn_71002e681c). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "game/animtree/CLookAtPosControl.h"
#include "types.h"

using base::reflection::UnkReflObject;

// As src/game/CEntity_2.cpp defines it.
struct UnkEntityPos;
UnkVec3f fn_7100d7b7dc(const UnkEntityPos* self);

// An object looked up by id: its transform at +0x38.
struct UnkLookAtTarget_b2e55e8 : UnkReflObject {
    u8 pad_0x8[0x30];
    UnkXform xform;
};

struct UnkLookAtName_b2e55e8 {
    u8 pad_0x0[0x30];
    u64 id;  // the entity to look at
};

struct UnkLookAtDef_b2e55e8 {
    u8 pad_0x0[0x40];
    UnkLookAtName_b2e55e8* field_0x40;
};

struct UnkLookAtEntityControl_b2e55e8 {
    u8 pad_0x0[0x10];
    UnkLookAtDef_b2e55e8* def;
};

void fn_71002e681c(UnkLookAtEntityControl_b2e55e8* self, f32 dt);

// An entity's position (fn_7100d7b7dc), else the translation of the object's transform.
static inline UnkVec3f PositionOf(UnkLookAtTarget_b2e55e8* target) {
    if (fn_710009d50c(target->vf_0x0(), fn_7100d70c20()))
        return fn_7100d7b7dc(reinterpret_cast<const UnkEntityPos*>(target));
    return target->xform.GetTranslation();
}

// The entity the definition names, looked up by its id (null when there is none).
inline UnkLookAtTarget_b2e55e8* LookAtTarget(UnkLookAtEntityControl_b2e55e8* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    u64 id = self->def->field_0x40->id;
    alignas(8) u32 kind = g_unk_71727bebb8;
    return static_cast<UnkLookAtTarget_b2e55e8*>(fn_7100deba8c(manager, &id, &kind));
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71002e55e8(UnkLookAtEntityControl_b2e55e8* self, f32 dt) {
    UnkLookAtTarget_b2e55e8* target = LookAtTarget(self);
    UnkVec3f pos = target ? PositionOf(target) : UnkVec3f(0.0f, 0.0f, 0.0f);
    CLookAtPosControl_SetTargetPos(reinterpret_cast<UnkLookAtPosControl_b69*>(self), reinterpret_cast<const base::math::CVector3D&>(pos),
                  -1.0f);
    fn_71002e681c(self, dt);
}
