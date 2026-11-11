// behaviortree::CScorpiusRegereratingMaskAction: ending the action. After the base step, hands the
// owner's target (+0x298) a kind-5 block facing along the third column of the owner's matrix, stops
// listening (the link at +0x38) through the weak handle at +0x50 and resets the tunable value at
// +0xac of g_unk_7172790a08 to 1. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CCharClassTweakPlayRateTrack.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The kind-5 block (game_s2/UnkParams.h) with its first fields seen as words.
struct UnkMaskParams {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkMaskTunable {
    u8 pad_0x0[0xac];
    f32 field_0xac;
};

struct UnkGlobal2;

struct UnkMaskAction {
    u8 pad_0x0[0x18];
    UnkActor* owner;
    u8 pad_0x20[0x18];
    u8 link[0x18];  // 0x38
    UnkWeakHandle* handle;  // 0x50
};

static inline u32 AsU32(const f32& value) {
    u32 bits;
    __builtin_memcpy(&bits, &value, 4);
    return bits;
}

void fn_710068cf4c(UnkMaskAction* self) {
    fn_7100394bd4(self);
    UnkActor* owner = self->owner;
    if (owner) {
        if (void* target = owner->field_0x298) {
            UnkMaskParams params;
            __builtin_memset(&params.field_0x8, 0, 13);
            params.field_0x30 = 0;
            __builtin_memset(params.field_0x20, 0, 16);
            params.vtable = g_unk_7101a25b10 + 2;
            params.field_0x39 = 0;
            params.field_0x3b = 0;
            params.field_0x3d = 0;
            params.field_0x34 = 5;
            params.field_0x18 = 0;
            params.field_0x1c = 1;
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
            u32 x = AsU32(m.m[2]);
            u32 y = AsU32(m.m[6]);
            u32 z = AsU32(m.m[10]);
            params.field_0x8 = x;
            params.field_0xc = y;
            params.field_0x10 = z;
            params.field_0x14 = 1;
            fn_71006f4b0c(target, &params, 0);
        }
    }
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->link);
        ResetRef(&self->handle);
    }
    reinterpret_cast<UnkMaskTunable*>(&g_unk_7172790a08)->field_0xac = 1.0f;
}
