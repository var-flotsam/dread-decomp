// behaviortree::CShakernautChaseTargetAction's unit (tools/tu_map.py unit 1043): fn_71006aa9e0 starts the
// action: after the base start (fn_7100394bd0) it moves to state 2 (telling the AI, +0x14518), clears its
// counters, marks the Shakernaut AI (+0x148a1) and takes the chase range: the action's own pair (+0x6c,
// +0x70) unless either is -1, else the path source's (the AI's +0x1610, vf_0x90); the AI's range object
// (+0x1148) may override it (vf_0xb8), and the range is restored when both come back negative. Names and
// layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

struct UnkStruct_0x68;

// The chase path source (the AI's +0x1610).
struct UnkShakernautPathSource_ovb0136 {
    void** vtable;
    u8 pad_0x8[0x11c];
    u32 field_0x124;

    void vf_0x90(void* owner, f32* a, f32* b) {
        reinterpret_cast<void (*)(UnkShakernautPathSource_ovb0136*, void*, f32*, f32*)>(vtable[0x90 / 8])(this, owner,
                                                                                                         a, b);
    }
};

// The object at the AI's +0x1148 that may override the chase range.
struct UnkShakernautRange_ovb0136 {
    void** vtable;
    void vf_0xb8(u32 kind, f32* a, f32* b) {
        reinterpret_cast<void (*)(UnkShakernautRange_ovb0136*, u32, f32*, f32*)>(vtable[0xb8 / 8])(this, kind, a, b);
    }
};

struct UnkShakernautAI_ovb0136 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1138];
    u32 field_0x1140;
    u8 pad_0x1144[4];
    UnkShakernautRange_ovb0136* field_0x1148;
    u8 pad_0x1150[0x4c0];
    UnkShakernautPathSource_ovb0136* field_0x1610;
    u8 pad_0x1618[0x14518 - 0x1618];
    s32 field_0x14518;
    u8 pad_0x1451c[0x148a1 - 0x1451c];
    bool field_0x148a1;
};

struct UnkShakernautOwner_ovb0136 {
    u8 pad_0x0[0x240];
    UnkShakernautAI_ovb0136* ai;
};

struct UnkShakernautChase_ovb0136 {
    u8 pad_0x0[0x18];
    UnkShakernautOwner_ovb0136* owner;  // 0x18
    u8 pad_0x20[0xc];
    u32 field_0x2c[7];  // 0x2c
    u32 field_0x48;
    s32 field_0x4c;
    s32 field_0x50;
    u32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    u8 pad_0x60[0xc];
    f32 field_0x6c;
    f32 field_0x70;
};

void fn_71006aa9e0(UnkShakernautChase_ovb0136* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->field_0x4c = 0;
    if (__builtin_expect(self->field_0x50 != 2, 1)) {
        self->field_0x4c = 0;
        self->field_0x50 = 2;
        if (UnkShakernautOwner_ovb0136* owner = self->owner) {
            if (UnkShakernautAI_ovb0136* ai = owner->ai) {
                // Stored untyped: the original reloads the owner afterwards.
                s32 state = 2;
                __builtin_memcpy(&ai->field_0x14518, &state, sizeof(state));
            }
        }
    }
    for (u32 i = 0; i < 7; i++)
        self->field_0x2c[i] = 0;
    if (!self->owner)
        return;
    UnkShakernautAI_ovb0136* ai = self->owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71006ad0a4()))
        return;
    ai->field_0x148a1 = true;
    UnkShakernautOwner_ovb0136* owner = self->owner;
    if (!owner || !owner->ai)
        return;
    UnkShakernautPathSource_ovb0136* source = owner->ai->field_0x1610;
    if (!source)
        return;
    UnkShakernautRange_ovb0136* range = ai->field_0x1148;
    u32 kind = ai->field_0x1140;
    self->field_0x54 = source->field_0x124;
    f32 a = 0.0f;
    f32 b = 0.0f;
    if (self->field_0x6c != -1.0f && self->field_0x70 != -1.0f) {
        a = self->field_0x6c;
        b = self->field_0x70;
    } else {
        source->vf_0x90(owner, &a, &b);
    }
    self->field_0x58 = a;
    self->field_0x5c = b;
    if (!range)
        return;
    range->vf_0xb8(kind, &self->field_0x58, &self->field_0x5c);
    if (self->field_0x58 < 0.0f && self->field_0x5c < 0.0f) {
        self->field_0x58 = a;
        self->field_0x5c = b;
    }
}
