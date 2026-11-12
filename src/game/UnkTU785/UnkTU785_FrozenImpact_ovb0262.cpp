// Unit 785: plays the frozen-impact effect on the player the attack's actor acts against (the sibling
// of src/game/UnkTU2666/UnkTU2666_lua_ovb0163.cpp's impact requests): unless the player is already
// frozen, a request with a static id, the effect id "frozen_impact" (a function-local static) and
// a direction (the sign of the offset from the owner's marker, then 1) is sent to the player's component
// at +0x1b8 (virtual 0x188). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU785.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
struct UnkRequest;

struct UnkImpactRequest_ovb0262y {
    u8 pad_0x0[0x18];
    bool field_0x18;
    u8 pad_0x19[0xa];
    bool field_0x23;
    u8 pad_0x24[0x25];
    bool field_0x49;
    u8 pad_0x4a[2];
    f32 dir_x;  // 0x4c
    f32 dir_y;  // 0x50
    u8 pad_0x54[0x4c];
    u8 effect_raw[8];  // 0xa0: a CStrId
    u8 pad_0xa8[0x10];
    u8 id_raw[8];  // 0xb8: a CStrId
    u8 pad_0xc0[0x40];

    CStrId& effect() { return *reinterpret_cast<CStrId*>(effect_raw); }
    CStrId& id() { return *reinterpret_cast<CStrId*>(id_raw); }

    UnkImpactRequest_ovb0262y() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkImpactRequest_ovb0262y() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};

struct UnkImpactPlayerTarget_ovb0262y {
    void** vtable;
    void vf_0x188(UnkImpactRequest_ovb0262y* request) {
        reinterpret_cast<void (*)(UnkImpactPlayerTarget_ovb0262y*, UnkImpactRequest_ovb0262y*)>(
            vtable[0x188 / 8])(this, request);
    }
};

struct UnkImpactVictim_ovb0262y {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x150];
    UnkImpactPlayerTarget_ovb0262y* target;  // 0x1b8
    u8 pad_0x1c0[0x40];
    u8* state;  // 0x200
};

struct UnkImpactOwner_ovb0262y {
    u8 pad_0x0[0x148c8];
    f32 marker_x;  // 0x148c8
};

struct UnkImpactAttack_ovb0262y {
    UnkImpactOwner_ovb0262y* owner;  // 0x0
    UnkActor* actor;                 // 0x8
    u8 pad_0x10[8];
    u32 field_0x18;
};

void fn_71004f3aa4(UnkImpactAttack_ovb0262y* self) {
    UnkImpactVictim_ovb0262y* victim = reinterpret_cast<UnkImpactVictim_ovb0262y*>(fn_7100d7c990(self->actor));
    if (!victim || !victim->target || !victim->state || victim->state[0x18b0])
        return;
    base::math::CVector2D flat;
    f32 px = victim->x, py = victim->y, pz = victim->z;
    UnkImpactOwner_ovb0262y* owner = self->owner;
    {
        base::math::CVector3D position = {px, py, pz};
        fn_710007b430(&flat, &position);
    }
    f32 offset = flat.x - owner->marker_x;
    UnkImpactRequest_ovb0262y request;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), 0.0f);
    request.id() = *reinterpret_cast<CStrId*>(&g_unk_71726dbf68);
    request.field_0x18 = true;
    request.field_0x23 = true;
    request.field_0x49 = true;
    request.dir_x = static_cast<f32>((offset > 0.0f) - (offset < 0.0f));
    request.dir_y = 1.0f;
    static CStrId s_effect("frozen_impact", true);
    request.effect() = s_effect;
    victim->target->vf_0x188(&request);
    self->field_0x18 = 0;
}
