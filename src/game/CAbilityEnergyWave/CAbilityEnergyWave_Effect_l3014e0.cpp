// CAbilityEnergyWave unit: tints `target` (slot 0xc8) with a colour, then claims the effect with hash
// 0x6923d83cbb01adc0 in the owner's world and plays it on all its parts. Names and layouts are placeholders.
#include "game/CFXComponent.h"
#include "game/CAbilityLayout.h"
#include "types.h"

struct UnkVec4_l3014e0 {
    f32 r, g, b, a;
};

struct UnkTarget_l3014e0 {
    void** vtable;
    void vf_0xc8(UnkVec4_l3014e0* tint) { reinterpret_cast<void (*)(UnkTarget_l3014e0*, UnkVec4_l3014e0*)>(vtable[25])(this, tint); }
};

struct UnkEffect_l3014e0 {
    void** vtable;
    u8 pad_0x8[3];
    u8 field_0xb;
    void vf_0x90(u32 a) { reinterpret_cast<void (*)(UnkEffect_l3014e0*, u32)>(vtable[18])(this, a); }
    void vf_0xd0(u32 a, u32 b, UnkVec4_l3014e0* v, u32 c) {
        reinterpret_cast<void (*)(UnkEffect_l3014e0*, u32, u32, UnkVec4_l3014e0*, u32)>(vtable[26])(this, a, b, v, c);
    }
    s32 vf_0xf0() { return reinterpret_cast<s32 (*)(UnkEffect_l3014e0*)>(vtable[30])(this); }
};

struct UnkWorld;

struct UnkOwner_l3014e0 {
    u8 pad_0x0[0x280];
    UnkWorld* field_0x280;
};

struct UnkObject;

UnkEffect_l3014e0* fn_71003014e0(CAbilityComponent* self, UnkTarget_l3014e0* target) {
    UnkVec4_l3014e0 tint;
    tint.r = tint.g = tint.b = tint.a = 1.0f;
    if (target)
        target->vf_0xc8(&tint);
    UnkWorld* world = self->Owner<UnkOwner_l3014e0>()->field_0x280;
    UnkEffect_l3014e0* part = reinterpret_cast<UnkEffect_l3014e0*>(fn_7101152b90(world, 0x6923d83cbb01adc0ull));
    if (part) {
        if (fn_710114e668(reinterpret_cast<UnkObject*>(world), part->field_0xb)) {
            fn_710114fe8c(world, 0x6923d83cbb01adc0ull, 1, 1);
            part->vf_0x90(0);
            s32 count = part->vf_0xf0();
            for (s32 i = 0; i < count; ++i)
                part->vf_0xd0(i, 0, &tint, 1);
        } else {
            part = nullptr;
        }
    }
    return part;
}
