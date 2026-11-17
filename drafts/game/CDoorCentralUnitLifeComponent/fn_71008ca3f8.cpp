// CDoorCentralUnitLifeComponent unit (tools/tu_map.py): the hit handler. Answers 2 in state 3; otherwise
// the owner's part answers first, then an impact of the right kind runs vf_0x1e0 and sends
// "CENTRAL_UNIT_DOOR_IMPACT"; anything else goes to the base handler. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU794.h"
#include "types.h"

using base::global::CStrId;

struct UnkDoorHitSource_l8ca3f8 {
    u8 pad_0x0[0x18];
    u64 id;  // 0x18
};

struct UnkDoorHit_l8ca3f8 {
    u8 pad_0x0[0x60];
    UnkDoorHitSource_l8ca3f8* source;  // 0x60
    u8 pad_0x68[0xb4 - 0x68];
    u32 flags;  // 0xb4
};

struct UnkDoorHitKind_l8ca3f8 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkDoorLifeState_l8ca3f8 {
    u8 pad_0x0[0x10];
    s32 id;  // 0x10
};

struct UnkDoorLife_l8ca3f8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkDoorLifeOwner_b56* owner;  // 0x30
    u8 pad_0x38[0x430 - 0x38];
    UnkDoorLifeState_l8ca3f8* state;  // 0x430

    u32 vf_0x1e0(UnkDoorHit_l8ca3f8* hit) {
        return reinterpret_cast<u32 (*)(UnkDoorLife_l8ca3f8*, UnkDoorHit_l8ca3f8*)>(vtable[0x1e0 / 8])(this, hit);
    }
};

// The arguments of the impact message: none.
struct UnkImpactArgs_l8ca3f8 {
    u64 field_0x0;
    u32 field_0x8;

    UnkImpactArgs_l8ca3f8() : field_0x0(0), field_0x8(0) {}
};

__attribute__((visibility("hidden"))) extern UnkDoorHitKind_l8ca3f8* g_unk_7172730f68;

u32 fn_710086bf60(UnkDoorLifePart_b56* part, UnkDoorHit_l8ca3f8* hit);
u32 fn_71008d3e44(UnkDoorLife_l8ca3f8* self, UnkDoorHit_l8ca3f8* hit);

u32 fn_71008ca3f8(UnkDoorLife_l8ca3f8* self, UnkDoorHit_l8ca3f8* hit) {
    if (self->state && self->state->id == 3)
        return 2;
    u32 flags = hit->flags;
    bool impact = (flags & 0xe) && (flags & 0x80);
    if (UnkDoorLifePart_b56* part = fn_71005012bc(self->owner)) {
        if (u32 result = fn_710086bf60(part, hit))
            return result;
    }
    if (impact && hit->source->id == g_unk_7172730f68->id) {
        u32 result = self->vf_0x1e0(hit);
        CStrId name("CENTRAL_UNIT_DOOR_IMPACT", true);
        UnkGameManager* manager = g_unk_7101d47310;
        UnkImpactArgs_l8ca3f8 args;
        fn_7100e038b0(manager, &name, reinterpret_cast<UnkVec*>(&args), 0);
        return result;
    }
    return fn_71008d3e44(self, hit);
}
