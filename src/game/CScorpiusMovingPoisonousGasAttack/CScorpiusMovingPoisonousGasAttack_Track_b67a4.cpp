// CScorpiusMovingPoisonousGasAttack's unit (tools/tu_map.py): the track listener that
// CScorpiusMovingPoisonousGasAttack_b38.cpp's start links (context = the attack). On an event of kind 2
// whose source's descriptor (+0x8 -> +0x10 -> +0x38) is g_unk_7172703310, the owner's target (+0x298)
// gets a kind-5 parameter block (game_s2/UnkParams.h) facing along the third column of the owner's
// matrix. Always answers true. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "types.h"

extern __attribute__((visibility("hidden"))) void* g_unk_7172703310;

struct UnkGasTrackDesc_b67a4 {
    u8 pad_0x0[0x38];
    void* kind;  // 0x38
};

struct UnkGasTrackSource_b67a4 {
    u8 pad_0x0[0x10];
    UnkGasTrackDesc_b67a4* desc;  // 0x10
};

struct UnkGasTrackEvent_b67a4 {
    u32 type;
    u8 pad_0x4[4];
    UnkGasTrackSource_b67a4* source;  // 0x8
};

// The kind-5 block with its first fields seen as words (as the b38 file's UnkGasParams).
struct UnkGasTrackParams_b67a4 {
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

struct UnkGasTrackAttack_b67a4 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

static inline u32 AsU32_b67a4(const f32& value) {
    u32 bits;
    __builtin_memcpy(&bits, &value, 4);
    return bits;
}

bool fn_71006895a0(UnkGasTrackAttack_b67a4* self, const UnkGasTrackEvent_b67a4* event) {
    if (event->type == 2 && event->source->desc->kind == g_unk_7172703310) {
        UnkActor* owner = self->owner;
        if (owner) {
            if (void* target = owner->field_0x298) {
                UnkGasTrackParams_b67a4 params;
                params.field_0x1c = 0;
                __builtin_memset(&params.field_0x8, 0, 13);
                params.field_0x30 = 0;
                __builtin_memset(params.field_0x20, 0, 16);
                params.vtable = g_unk_7101a25b10 + 2;
                params.field_0x39 = 0;
                params.field_0x3b = 0;
                params.field_0x3d = 0;
                params.field_0x34 = 5;
                const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
                u32 x = AsU32_b67a4(m.m[2]);
                u32 y = AsU32_b67a4(m.m[6]);
                u32 z = AsU32_b67a4(m.m[10]);
                params.field_0x8 = x;
                params.field_0xc = y;
                params.field_0x10 = z;
                params.field_0x14 = 1;
                fn_71006f4b0c(target, &params, 0);
            }
        }
    }
    return true;
}
