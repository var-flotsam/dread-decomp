// State checksum of an object: tag 0xc, a 64-bit value reached through its first pointer, the
// hash of one reference and the hash of an embedded sub-object.
#include "game/UnkHashStream.h"

struct UnkInner_0x38 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkHashed_71000e81e8 {
    UnkInner_0x38* inner_0x0;
    u8 pad_0x8[8];
    void* ref_0x10;
    u8 pad_0x18[0x20];
    u8 sub_0x38[8];
};

u32 fn_710009ebb4(void* ref);
u32 fn_7100093bb8(void* sub);

s32 fn_71000e81e8(UnkHashed_71000e81e8* self) {
    UnkScopedHashStream s(0x16, nullptr);
    s.Begin(0xc);
    s.Put(self->inner_0x0->field_0x30);
    s.Put(fn_710009ebb4(self->ref_0x10));
    s.Put(fn_7100093bb8(self->sub_0x38));
    return s.Finish();
}
