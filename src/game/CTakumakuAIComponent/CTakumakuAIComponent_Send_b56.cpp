// Two sends in the CTakumakuAIComponent unit, both under kind 0xbb2ee0d16add63b8 with mode 0 to the
// owner's sub-object at +0x220 (descriptors as UnkSpitSend, src/game_s2/UnkSpitSend.h), each
// followed by setting the timer triple at +0x14748 from the tunable at +0x146c8 (addressed from the tunable's slot, pinned). Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// As src/game/CAnimationComponent/fn_7101176ea8.cpp declares it (the key is a hash).
struct UnkStruct_7101176ea8;

struct UnkTakumakuEntry_b56a3 {
    u8 pad_0x0[0x28];
    f32 field_0x28;
};

struct UnkTakumakuTunable_b56a3 {
    u8 pad_0x0[0x61c];
    f32 field_0x61c;
    f32 field_0x620;
    u8 pad_0x624[8];
    f32 field_0x62c;
};

struct UnkTakumakuOwner_b56a3 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

// The tunable and the timer triple after it.
struct UnkTakumakuTimer_b56a3 {
    UnkTakumakuTunable_b56a3* tunable;
    u8 pad_0x8[0x78];
    f32 field_0x80;
    f32 field_0x84;
    f32 field_0x88;
};

struct UnkTakumakuAI_b56a3 {
    u8 pad_0x0[0x30];
    UnkTakumakuOwner_b56a3* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    UnkTakumakuTimer_b56a3 timer;  // 0x146c8
};

static inline __attribute__((always_inline)) bool Send_b56a3(UnkAnimationComponent* target, u64 hash) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, 0xbb2ee0d16add63b8, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

// When the sub-object has the entry 0x11d3e6cd2e9cb1a1 and the tunable is set, sends it; when that
// was taken, the timer starts from the entry's +0x28 with the tunable's +0x62c and scale one.
void fn_71006d5bf8(UnkTakumakuAI_b56a3* self) {
    if (!self->owner)
        return;
    UnkAnimationComponent* target = self->owner->field_0x220;
    if (!target)
        return;
    UnkTakumakuTimer_b56a3* tp = &self->timer;
    auto* entry = static_cast<UnkTakumakuEntry_b56a3*>(
        fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(target), reinterpret_cast<void*>(0x11d3e6cd2e9cb1a1)));
    UnkTakumakuTunable_b56a3* tunable = tp->tunable;
    if (!entry || !tunable)
        return;
    if (Send_b56a3(target, 0x11d3e6cd2e9cb1a1)) {
        tp->field_0x84 = tp->tunable->field_0x62c;
        tp->field_0x88 = 1.0f;
        tp->field_0x80 = entry->field_0x28;
    }
}

// While the tunable's +0x61c is above 0.001: sends 0x33cb81bf00f5b762 unless the sub-object already
// plays it (fn_710117ae38), then the timer starts from that value with the tunable's +0x620 and
// scale 0.75.
void fn_71006d5e54(UnkTakumakuAI_b56a3* self) {
    UnkTakumakuTimer_b56a3* tp = &self->timer;
    UnkAnimationComponent* target = self->owner ? self->owner->field_0x220 : nullptr;
    UnkTakumakuTunable_b56a3* tunable = tp->tunable;
    if (!tunable)
        return;
    asm volatile("" : "+r"(tunable));
    if (!target)
        return;
    f32 t = tunable->field_0x61c;
    if (!(t > 0.001f))
        return;
    if (!fn_710117ae38(target, 0x33cb81bf00f5b762, false))
        Send_b56a3(target, 0x33cb81bf00f5b762);
    tp->field_0x84 = tp->tunable->field_0x620;
    tp->field_0x88 = 0.75f;
    tp->field_0x80 = t;
}
