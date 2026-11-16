// CCentralUnitCannonAIComponent's unit (tools/tu_map.py): spawns the cannon's beam actor and places it at the
// owner's muzzle (the model's node when found, else the owner's position facing its z axis), then aims its
// movement component (when it is a CCharClassCentralUnitCannonBeamMovementComponent). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkEntity;

struct alignas(8) UnkPowerUpLifeType_b52 {
    u32 id;
};

struct UnkCannonAI_l3fd9fc {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x146cc - 0x38];
    u32 area;  // 0x146cc
};

struct UnkCannonBeam_l3fd9fc {
    void** vtable;

    void SetPosition(const UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkCannonBeam_l3fd9fc*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
    void SetAngle(const UnkVec3f* angle, bool flag) {
        reinterpret_cast<void (*)(UnkCannonBeam_l3fd9fc*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(this, angle, flag);
    }
};

struct UnkBeamMovement_l3fd9fc {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x28];
    UnkVec3f dir;  // 0x40

    void vf_0x290(UnkActor* owner) {
        reinterpret_cast<void (*)(UnkBeamMovement_l3fd9fc*, UnkActor*)>(vtable[0x290 / 8])(this, owner);
    }
    void vf_0x2c0() { reinterpret_cast<void (*)(UnkBeamMovement_l3fd9fc*)>(vtable[0x2c0 / 8])(this); }
};

struct UnkRefl_fn_71003fe0f8;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_71003fe0f8>(void*);

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726c3108;
extern char g_unk_71016493c8[];

void fn_71004005c8(UnkBeamMovement_l3fd9fc* movement, u32 area);

bool fn_71003fd9fc(UnkCannonAI_l3fd9fc* self) {
    UnkPowerUpLifeType_b52 type;
    UnkActorDefFactory* factory = g_unk_71726a0220;
    ++g_unk_71726c3108.data->reads;
    const char* text = reinterpret_cast<const char*>(&g_unk_71726c3108.data->String());
    u32 type_id = g_unk_7172a0ba48;
    type.id = type_id;
    auto* beam = reinterpret_cast<UnkCannonBeam_l3fd9fc*>(
        fn_710119438c(factory, text, &g_unk_71726c3108, type, nullptr));
    if (!beam)
        return false;

    UnkActor* owner = self->owner;
    UnkVec3f pos(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    const UnkMatrix44& xform = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    UnkVec3f dir(xform.m[2], xform.m[6], xform.m[10]);
    UnkMatrix44 muzzle = {};
    muzzle.m[15] = 1.0f;
    muzzle.m[10] = 1.0f;
    muzzle.m[5] = 1.0f;
    muzzle.m[0] = 1.0f;
    if (fn_71011735e8(static_cast<UnkEntity*>(self->owner->field_0x228), g_unk_71016493c8, &muzzle, 0)) {
        pos.x = muzzle.m[3];
        pos.y = muzzle.m[7];
        pos.z = muzzle.m[11];
        dir.x = muzzle.m[0];
        dir.y = muzzle.m[4];
        dir.z = muzzle.m[8];
    }
    UnkVec3f angle = fn_710007a7f0(dir);
    beam->SetPosition(&pos, true);
    beam->SetAngle(&angle, true);

    auto* movement = reinterpret_cast<UnkBeamMovement_l3fd9fc*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_71003fe0f8>(beam));
    if (!movement)
        return false;
    UnkReflObject* char_class = movement->char_class;
    if (!char_class)
        return false;
    if (!fn_710009d50c(char_class->vf_0x0(), fn_7100400d04()))
        return false;
    movement->dir = dir;
    movement->vf_0x290(self->owner);
    fn_71004005c8(movement, self->area);
    movement->vf_0x2c0();
    return true;
}
