// CChozoZombieXPoisonClawsAttack's unit (tools/tu_map.py unit 735): the attack's damage set-up, as
// src/game/CYamplotXBiteAttack/CYamplotXAttack_Damage_b65a4.cpp with one string id: unless the owner's
// (+0x8) component at +0x2c8 is frozen (fn_71007f9f20), the request's id at +0xb8 becomes the unit's
// static id, its value (fn_71008c1538) is what fn_7100789f5c gives for that id, the word at +0xc8 is set
// to 1 and the byte at +0xd4 cleared. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CFrozenComponent.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_71726d1710 __attribute__((visibility("hidden")));

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);
f32 fn_7100789f5c(const CStrId* id);

struct UnkPoisonClawsOwner_A2_b71 {
    u8 pad_0x0[0x2c8];
    void* field_0x2c8;
};

struct UnkPoisonClawsAttack_A2_b71 {
    u8 pad_0x0[0x8];
    UnkPoisonClawsOwner_A2_b71* owner;  // 0x8
};

struct UnkPoisonClawsRequest_A2_b71 {
    u8 pad_0x0[0xb8];
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 field_0xc8;
    u8 pad_0xcc[0x8];
    bool field_0xd4;
};

void fn_710049ec34(UnkPoisonClawsAttack_A2_b71* self, void* unused, UnkPoisonClawsRequest_A2_b71* request) {
    void* frozen = self->owner->field_0x2c8;
    if (frozen && fn_71007f9f20(frozen))
        return;
    request->id = g_unk_71726d1710;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(request), fn_7100789f5c(&g_unk_71726d1710));
    request->field_0xc8 = 1;
    request->field_0xd4 = false;
}
