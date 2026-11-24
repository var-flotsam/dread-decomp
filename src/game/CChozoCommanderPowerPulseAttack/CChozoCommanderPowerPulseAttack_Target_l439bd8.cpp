// CChozoCommanderPowerPulseAttack's unit (tools/tu_map.py unit 655): where the pulse goes (fn_7100439bd8).
// An owner facing a side of the commander's range (+0x15d1c, ends at +0 and +8) closer to that end than the
// attack's distance (+0x10c of its definition) targets its own position moved by that distance away from
// the end; otherwise the ground attack's choice (fn_710042bb48) decides. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CChozoCommanderGroundAttack.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSelf_710042bb48;
struct UnkOut_42bb48;

struct UnkPulseRange_l439bd8 {
    f32 left;
    f32 pad_0x4;
    f32 right;
};

struct UnkPulseAI_l439bd8 {
    u8 pad_0x0[0x15d1c];
    UnkPulseRange_l439bd8 range;  // 0x15d1c
};

struct UnkPulseDef_l439bd8 {
    u8 pad_0x0[0x10c];
    f32 distance;  // 0x10c
};

struct UnkPulseAttack_l439bd8 {
    void* vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x40];
    UnkPulseDef_l439bd8* def;  // 0x50
};

bool CChozoCommanderPowerPulseAttack_GetAttackPosKeepingMinDistToWall(UnkPulseAttack_l439bd8* self, UnkVec3f* out) {
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    UnkReflObject* commander = ai && fn_710009d50c(ai->vf_0x0(), fn_710044e708()) ? ai : nullptr;
    UnkPulseRange_l439bd8* range = &reinterpret_cast<UnkPulseAI_l439bd8*>(commander)->range;
    UnkActor* owner = self->owner;
    f32 x = owner->field_0x44;
    f32 facing = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38)).m[2];
    if (facing < 0.0f && x - range->left < self->def->distance) {
        owner = self->owner;
        *out = UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64);
        out->x = self->def->distance + out->x;
        return true;
    }
    if (facing > 0.0f && range->right - x < self->def->distance) {
        owner = self->owner;
        *out = UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64);
        out->x = -self->def->distance + out->x;
        return true;
    }
    return CChozoCommanderGroundAttack_ComputeDesiredAttackPos(reinterpret_cast<UnkSelf_710042bb48*>(self), reinterpret_cast<UnkOut_42bb48*>(out));
}
