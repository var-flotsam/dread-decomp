// State checksum of an object: tag 0x14, a 64-bit value from its first reference, the same
// value from an optional second one (-1 if absent) and the hash of an optional third (-1).
#include "base/UnkHashStream.h"

struct UnkInner {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkNode {
    u8 pad_0x0[8];
    UnkInner* inner_0x8;
};

struct UnkHashed_710009d110 {
    u8 pad_0x0[8];
    UnkInner* inner_0x8;
    u8 pad_0x10[0x68];
    UnkNode* node_0x78;
    void* ref_0x80;
};

u32 fn_7100093bb8(void* ref);

u32 fn_710009d110(UnkHashed_710009d110* self) {
    UnkScopedHashStream s(0x16, nullptr);
    s.Begin(0x14);
    s.Put(self->inner_0x8->field_0x30);
    s.Put(self->node_0x78 ? self->node_0x78->inner_0x8->field_0x30 : (u64)-1);
    s.Put(self->ref_0x80 ? fn_7100093bb8(self->ref_0x80) : (u32)-1);
    return s.Finish();
}
