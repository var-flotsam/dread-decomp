// CBasicLifeComponent's unit (tools/tu_map.py unit 1354): the hit handler. Scales the hit by the damage factors, lets the
// component accept (vf_0x1d8, vf_0x1e0) or reject (vf_0x1e8) it, otherwise resolves it (vf_0x1a8). Names and layouts
// are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "game/CActorComponent.h"
#include "game/CDamageSourceFactor.h"
#include "game/UnkTU1370.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkRobotSoldierTable_A2_b69;
struct UnkRequest;
extern u64 g_unk_710164af70;

struct UnkLifeHit_ovb0265 {
    u8 pad_0x0[0xb4];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0xf8 - 0xb8];
    f32 amount;  // 0xf8
};

struct UnkLifeActor_ovb0265 {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
    u8 pad_0x1b6[0x2e8 - 0x1b6];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkLifeConfig_ovb0265 {
    u8 pad_0x0[0xa8];
    u8 factors[0x11c - 0xa8];  // 0xa8
    f32 field_0x11c;
    f32 field_0x120;
};

struct UnkBasicLife_ovb0265 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkLifeActor_ovb0265* owner;  // 0x30
    u8 pad_0x38[0x54 - 0x38];
    f32 field_0x54;
    u8 pad_0x58[0x64 - 0x58];
    bool field_0x64;
    u8 pad_0x65[0x70 - 0x65];
    s32 field_0x70;
    u8 pad_0x74[0x268 - 0x74];
    UnkLifeConfig_ovb0265* config;  // 0x268
    u8 pad_0x270[8];
    f32 field_0x278;

    bool Allows(UnkLifeHit_ovb0265* hit) {
        return reinterpret_cast<bool (*)(UnkBasicLife_ovb0265*, UnkLifeHit_ovb0265*)>(vtable[0x1d8 / 8])(this, hit);
    }
    u32 Handle(UnkLifeHit_ovb0265* hit) {
        return reinterpret_cast<u32 (*)(UnkBasicLife_ovb0265*, UnkLifeHit_ovb0265*)>(vtable[0x1e0 / 8])(this, hit);
    }
    u32 Resolve(UnkLifeHit_ovb0265* hit) {
        return reinterpret_cast<u32 (*)(UnkBasicLife_ovb0265*, UnkLifeHit_ovb0265*)>(vtable[0x1a8 / 8])(this, hit);
    }
    void Reject(UnkLifeHit_ovb0265* hit) {
        reinterpret_cast<void (*)(UnkBasicLife_ovb0265*, UnkLifeHit_ovb0265*)>(vtable[0x1e8 / 8])(this, hit);
    }
    void Result(u32 value, UnkLifeHit_ovb0265* hit) {
        reinterpret_cast<void (*)(UnkBasicLife_ovb0265*, u32, UnkLifeHit_ovb0265*)>(vtable[0x1d0 / 8])(this, value, hit);
    }
};

u32 fn_71008a3c48(UnkBasicLife_ovb0265* self, UnkLifeHit_ovb0265* hit) {
    if (self->field_0x64 || self->field_0x70 == 2 || self->owner->field_0x1b5)
        return 0;
    if (fn_710024fbc8(self)) {
        UnkLifeConfig_ovb0265* config = self->config;
        f32 factor = fn_71008c2de8(reinterpret_cast<UnkRobotSoldierTable_A2_b69*>(config->factors), hit->flags);
        fn_71008c1538(reinterpret_cast<UnkRequest*>(hit), factor * hit->amount);
        config = self->config;
        if (self->field_0x278 < config->field_0x120)
            fn_71008c1538(reinterpret_cast<UnkRequest*>(hit), hit->amount * config->field_0x11c);
        if (hit->flags >> 5 & 1)
            fn_71008c1538(reinterpret_cast<UnkRequest*>(hit), self->field_0x54);
        if (!self->Allows(hit)) {
            // keeps the rejected path's vtable load separate from the handled path's
            asm("" : "+r"(hit));
            self->Reject(hit);
            return 2;
        }
        u32 handled = self->Handle(hit);
        if (handled)
            return handled;
    }
    u32 result = self->Resolve(hit);
    if (self->field_0x54 <= 0.0f) {
        self->Result(2, hit);
        return result;
    }
    if (result != 1)
        return result;
    if (UnkTimelineComponent* timeline = self->owner->timeline) {
        alignas(8) u32 mask = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
        fn_7100a2cb7c(timeline, &g_unk_710164af70, &mask, false, -1.0f);
    }
    return 1;
}
