// Checks the object behind +0x30 for a usable +0x208 and, if so, copies three of its values into `out`.
#include "game/UnkActor.h"
#include "types.h"

struct UnkInfo {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
    u8 pad_0x68[0x1a0];
    void* field_0x208;
};

struct UnkVec3 {
    u32 a;
    u32 b;
    u32 c;
};

struct UnkOut {
    u8 pad_0x0[0x14];
    u16 field_0x14;
    u8 field_0x16;
    u8 pad_0x17[9];
    u8 field_0x20[1];
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
};

bool fn_710110a164();
bool fn_71009e12d0(void* obj);
void fn_710007b444(void* dst, const UnkVec3* value);

bool fn_71003de7b4(UnkSelf* self, UnkOut* out) {
    bool result = fn_710110a164();
    UnkInfo* info = reinterpret_cast<UnkInfo*>(fn_7100d7c990(self->field_0x30));
    if (info && info->field_0x208 && fn_71009e12d0(info->field_0x208)) {
        u32 a = info->field_0x44;
        u32 b = info->field_0x54;
        u32 c = info->field_0x64;
        UnkVec3 v = {a, b, c};
        fn_710007b444(out->field_0x20, &v);
        out->field_0x16 = 0xff;
        out->field_0x14 = 0xffff;
        result = true;
    }
    return result;
}
