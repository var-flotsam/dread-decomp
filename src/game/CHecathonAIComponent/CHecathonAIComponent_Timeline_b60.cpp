// CHecathonAIComponent's unit (tools/tu_map.py): a pair that toggles a flag (+0x147d0) and the
// owner's timeline (+0x2e8) entry with it. Turning on sets id g_unk_710164a0a0 to the all-flags
// mask; turning off removes that id and sets g_unk_710164a0a8 instead (the set as
// src/game/CShelmitAIComponent/CShelmitAIComponent_Timeline_b59.cpp; the unused second argument is
// passed as false by fn_71005a9eac). fn_71005a9eac resets the attack: turns it off, clears the
// HECATHONPLANKTONFX child's intensity (fn_710080c9bc) unless +0x147c8 was set, removes the id
// g_unk_710164a0b8 once (+0x147d3), and draws two delays from the tunable g_unk_71726e8d00's ranges.
// Names are placeholders.
#include <stdlib.h>

#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkHecathonKey_b60;
extern UnkHecathonKey_b60 g_unk_710164a0a0;  // reached through the GOT
extern UnkHecathonKey_b60 g_unk_710164a0a8;  // reached through the GOT
extern UnkHecathonKey_b60 g_unk_710164a0b8;  // reached through the GOT
using base::reflection::UnkTunable;
extern UnkTunable g_unk_71726e8d00;

struct UnkHecathonAI_b60 {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x10];
    UnkActor* owner;
    u8 pad_0x38[0x14764 - 0x38];
    f32 field_0x14764;
    f32 field_0x14768;
    f32 field_0x1476c;
    f32 field_0x14770;
    f32 field_0x14774;
    f32 field_0x14778;
    u8 pad_0x1477c[0x147a4 - 0x1477c];
    f32 field_0x147a4;
    u8 pad_0x147a8[0x147b8 - 0x147a8];
    u32 field_0x147b8;
    u8 pad_0x147bc[0x147c8 - 0x147bc];
    bool field_0x147c8;
    u8 pad_0x147c9[3];
    bool field_0x147cc;
    u8 pad_0x147cd;
    bool field_0x147ce;
    u8 pad_0x147cf;
    bool active;
    bool field_0x147d1;
    u8 pad_0x147d2;
    bool field_0x147d3;
};

struct UnkRefl_fn_71005aad7c;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_71005aad7c>(void*);
struct UnkPlanktonFX_b44a2;
void fn_710080c9bc(UnkPlanktonFX_b44a2* self);

static inline f32 Random_b60() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandomRange_b60(f32 lo, f32 hi) {
    f32 r = Random_b60();
    return lo + (hi - lo) * r;
}

static inline f32 Tunable_b60(u32 offset) {
    return *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_71726e8d00) + offset);
}

void fn_71005ab080(UnkHecathonAI_b60* self, bool) {
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (timeline && self->active) {
        self->active = false;
        fn_7100a2b7b8(timeline, reinterpret_cast<const u64*>(&g_unk_710164a0a0));
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a0a8), &mask.id, false, -1.0f);
    }
}

void fn_71005ab414(UnkHecathonAI_b60* self, bool) {
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (timeline && !self->active) {
        self->active = true;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a0a0), &mask.id, false, -1.0f);
    }
}

void fn_71005a9eac(UnkHecathonAI_b60* self) {
    fn_71005ab080(self, false);
    if (!self->field_0x147c8)
        fn_710080c9bc(reinterpret_cast<UnkPlanktonFX_b44a2*>(
            base::reflection::refl_FindTyped<UnkRefl_fn_71005aad7c>(self->field_0x18)));
    if (self->field_0x147d3) {
        auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
        self->field_0x147d3 = false;
        fn_7100a2b7b8(timeline, reinterpret_cast<const u64*>(&g_unk_710164a0b8));
    }
    self->field_0x147a4 = 0.0f;
    self->field_0x147b8 = 0;
    self->field_0x147c8 = true;
    self->field_0x147cc = false;
    self->field_0x14774 = RandomRange_b60(Tunable_b60(0x34), Tunable_b60(0x38));
    self->field_0x14768 = Tunable_b60(0x28);
    self->field_0x14778 = RandomRange_b60(Tunable_b60(0x3c), Tunable_b60(0x40));
    self->field_0x14764 = Tunable_b60(0x24);
    if (self->field_0x14770 == 0.0f)
        self->field_0x14770 = Tunable_b60(0x30);
}

