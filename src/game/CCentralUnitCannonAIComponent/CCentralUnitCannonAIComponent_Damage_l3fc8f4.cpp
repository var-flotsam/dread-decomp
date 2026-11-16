// CCentralUnitCannonAIComponent's unit (tools/tu_map.py): the cannon beam's damage set-up for a request. After the base
// step, the request's id (+0xb8) becomes the damage name of the area kind at +0x146cc (caves, magma, lab, forest,
// sanctuary) and its value the matching tunable; with the flag at +0x146d0 set the value is then cleared.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CCharClassAttackComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::CStrId;

struct UnkCannonAI_l3fc8f4 {
    u8 pad_0x0[0x146cc];
    u32 area;  // 0x146cc
    bool no_damage;  // 0x146d0
};

struct UnkCannonDamageRequest_l3fc8f4 {
    u8 pad_0x0[0xb8];
    CStrId id;  // 0xb8
};

// The beam damages of the attack tunable (g_unk_7172716160).
struct UnkCannonDamageTunable_l3fc8f4 {
    u8 pad_0x0[0x7c];
    f32 damage_caves;   // 0x7c
    f32 damage_magma;   // 0x80
    f32 damage_lab;     // 0x84
    f32 damage_forest;  // 0x88
    f32 damage_sanc;    // 0x8c
};

void fn_71003fc8f4(UnkCannonAI_l3fc8f4* self, void* arg, UnkCannonDamageRequest_l3fc8f4* request) {
    fn_7101105c90(reinterpret_cast<UnkKraidAI2*>(self), arg, reinterpret_cast<UnkKraidRequest*>(request));
    const UnkCannonDamageTunable_l3fc8f4* tunable =
        reinterpret_cast<const UnkCannonDamageTunable_l3fc8f4*>(&g_unk_7172716160);
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
    if (self->no_damage)
        fn_71008c1538(damage, 0.0f);
}
