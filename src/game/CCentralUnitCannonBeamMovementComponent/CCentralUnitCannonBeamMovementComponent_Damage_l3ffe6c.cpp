// CCentralUnitCannonBeamMovementComponent's unit (tools/tu_map.py): the beam's damage set-up for a request. The
// request's id (+0xb8) becomes the damage name of the area kind at +0x5d8 (caves, magma, lab, forest, sanctuary)
// and its value the matching tunable; when the owner's cannon AI has its no-damage flag set the value is then
// cleared. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassAttackComponent.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

// The cannon AI component, as far as its no-damage flag goes.
struct UnkCannonAI_l3ffe6c : UnkReflObject {
    u8 pad_0x8[0x146c8];
    bool no_damage;  // 0x146d0
};
static_assert(__builtin_offsetof(UnkCannonAI_l3ffe6c, no_damage) == 0x146d0, "layout");

struct UnkCannonOwner_l3ffe6c {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkCannonOwnerRef_l3ffe6c {
    u8 pad_0x0[0x10];
    UnkCannonOwner_l3ffe6c* owner;  // 0x10
};

struct UnkCannonBeamMovement_l3ffe6c {
    u8 pad_0x0[0x230];
    UnkCannonOwnerRef_l3ffe6c* owner_ref;  // 0x230
    u8 pad_0x238[0x3a0];
    u32 area;  // 0x5d8
};
static_assert(__builtin_offsetof(UnkCannonBeamMovement_l3ffe6c, area) == 0x5d8, "layout");

struct UnkCannonDamageRequest_l3ffe6c {
    u8 pad_0x0[0xb8];
    CStrId id;  // 0xb8
};

// The beam damages of the attack tunable (g_unk_7172716160).
struct UnkCannonDamageTunable_l3ffe6c {
    u8 pad_0x0[0x7c];
    f32 damage_caves;   // 0x7c
    f32 damage_magma;   // 0x80
    f32 damage_lab;     // 0x84
    f32 damage_forest;  // 0x88
    f32 damage_sanc;    // 0x8c
};

static inline bool IsCannonAI(UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), fn_71003fafa0());
}

void CCentralUnitCannonBeamMovementComponent_SetBeamDamage(UnkCannonBeamMovement_l3ffe6c* self, UnkCannonDamageRequest_l3ffe6c* request) {
    const UnkCannonDamageTunable_l3ffe6c* tunable =
        reinterpret_cast<const UnkCannonDamageTunable_l3ffe6c*>(&g_unk_7172716160);
    UnkRequest* damage = reinterpret_cast<UnkRequest*>(request);
    switch (self->area) {
    case 0: {
        static CStrId s_id("fDamageCentralUnitCannonBeamCaves", true);
        request->id = s_id;
        fn_71008c1538(damage, tunable->damage_caves);
        break;
    }
    case 1: {
        static CStrId s_id("fDamageCentralUnitCannonBeamMagma", true);
        request->id = s_id;
        fn_71008c1538(damage, tunable->damage_magma);
        break;
    }
    case 2: {
        static CStrId s_id("fDamageCentralUnitCannonBeamLab", true);
        request->id = s_id;
        fn_71008c1538(damage, tunable->damage_lab);
        break;
    }
    case 3: {
        static CStrId s_id("fDamageCentralUnitCannonBeamForest", true);
        request->id = s_id;
        // The original sets the forest damage twice.
        fn_71008c1538(damage, tunable->damage_forest);
        fn_71008c1538(damage, tunable->damage_forest);
        break;
    }
    case 4: {
        static CStrId s_id("fDamageCentralUnitCannonBeamSanc", true);
        request->id = s_id;
        fn_71008c1538(damage, tunable->damage_sanc);
        break;
    }
    }
    if (self->owner_ref && self->owner_ref->owner) {
        if (UnkReflObject* ai = self->owner_ref->owner->ai) {
            if (IsCannonAI(ai) && static_cast<UnkCannonAI_l3ffe6c*>(ai)->no_damage)
                fn_71008c1538(damage, 0.0f);
        }
    }
}
