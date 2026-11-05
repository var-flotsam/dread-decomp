// CChozoCommanderLandingSlamAttack's unit (tools/tu_map.py): after fn_7100437c44, hands the owner's (+0x8)
// navigation object (+0x298) a parameter block of the vtable-0x7101a25b38 kind (fn_71006f4b0c) with the
// optional at +0x10 set to 1.0 and the word at +0x34 to 1; the block's reflected value (+0x70) is
// destroyed afterwards. Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::UnkReflValue;

extern void* g_unk_7101a25b38[];

struct UnkSlamParams_A2_b72 {
    void** vtable;
    u64 field_0x8;
    f32 field_0x10;
    bool field_0x14;
    u8 pad_0x15[0x7];
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    u8 pad_0x38[0x4];
    bool field_0x3c;
    u8 pad_0x3d;
    bool field_0x3e;
    u8 pad_0x3f;
    bool field_0x40;
    u8 pad_0x41[0x7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue value;  // 0x70
    bool field_0x88;

    UnkSlamParams_A2_b72() {
        field_0x1c = false;
        vtable = g_unk_7101a25b38 + 2;
        field_0x8 = 0;
        field_0x3c = false;
        value = base::reflection::refl_EmptyValue();
        field_0x3e = false;
        field_0x40 = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x58 = false;
        field_0x88 = false;
        field_0x34 = 1;
        field_0x30 = true;
        field_0x10 = 1.0f;
        field_0x14 = true;
        __builtin_memset(field_0x48, 0, 16);
        __builtin_memset(field_0x20, 0, 16);
    }
    ~UnkSlamParams_A2_b72() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&value);
    }
};

struct UnkSlamOwner_A2_b72 {
    u8 pad_0x0[0x298];
    void* field_0x298;
};

struct UnkSlamAttack_A2_b72 {
    u8 pad_0x0[0x8];
    UnkSlamOwner_A2_b72* owner;
};

void fn_7100437c44(UnkSlamAttack_A2_b72* self);

void fn_71004385c4(UnkSlamAttack_A2_b72* self) {
    fn_7100437c44(self);
    UnkSlamParams_A2_b72 params;
    fn_71006f4b0c(self->owner->field_0x298, &params, 0);
}
