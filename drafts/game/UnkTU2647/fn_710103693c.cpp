// Unit 2647 (sound::CAudioPreset): the constructor of the preset bank: an asset base, two string ids,
// sixteen empty presets and the defaults of its settings. Names and layouts are placeholders.
#include <new>

#include "base/core/CAsset.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101cac748[];
void fn_7101036a9c(void* items);

struct UnkAssetBase_b0192 {
    void* vtable;
    u8 unk_0x8[0x50];

    UnkAssetBase_b0192() {
        fn_7100016ad8(this);
        vtable = g_unk_7101cac748 + 0x10;
    }
};

struct UnkAudioPresetBank_b0192 : UnkAssetBase_b0192 {
    CStrId id_a;      // 0x58
    CStrId id_b;      // 0x60
    u8 items[0x500];  // 0x68
    s32 field_0x568;
    s32 field_0x56c;
    s32 field_0x570;
    s32 field_0x574;
    u32 field_0x578;
    u32 field_0x57c;
    u64 field_0x580;
    f32 field_0x588;
    u32 field_0x58c;
    f32 field_0x590;
    f32 field_0x594;
    u64 field_0x598;
    u64 field_0x5a0;
    u32 field_0x5a8;
    f32 field_0x5ac;
    f32 field_0x5b0;
    f32 field_0x5b4;
    f32 field_0x5b8;
    f32 field_0x5bc;
    f32 field_0x5c0;
    u8 field_0x5c4[3];
    bool field_0x5c7;
    bool field_0x5c8;
    u8 field_0x5c9;

    UnkAudioPresetBank_b0192()
        : id_a(base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x29c]), id_b(g_unk_7101d470e0) {
        fn_7101036a9c(items);
        field_0x578 = 0xffffffff;
        field_0x588 = 1.0f;
        field_0x568 = 0x7fffffff;
        field_0x56c = 0x7fffffff;
        field_0x590 = 1.0f;
        field_0x594 = 1.0f;
        field_0x570 = 1;
        field_0x58c = 0;
        field_0x574 = 0;
        field_0x57c = 0;
        field_0x580 = 0;
        field_0x598 = 0;
        field_0x5a0 = 0;
        field_0x5a8 = 0;
        field_0x5ac = -1.0f;
        field_0x5b0 = -1.0f;
        field_0x5b4 = 0.6f;
        field_0x5b8 = 10.0f;
        field_0x5bc = 20.0f;
        field_0x5c0 = 10.0f;
        field_0x5c4[0] = 0;
        field_0x5c4[1] = 0;
        field_0x5c4[2] = 0;
        field_0x5c7 = true;
        field_0x5c8 = true;
        field_0x5c9 = 0;
    }
};
static_assert(__builtin_offsetof(UnkAudioPresetBank_b0192, field_0x5c4) == 0x5c4, "layout");

void fn_710103693c(UnkAudioPresetBank_b0192* self) {
    new (self) UnkAudioPresetBank_b0192();
}
