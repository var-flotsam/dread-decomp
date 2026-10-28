// Computes a hash over an object's state: opens a stream (kind 0x16), appends its id, a
// 64-bit value reached through a pointer chain and the u32 at +0xe0, and returns the digest of the buffer.
#include "base/UnkHashStream.h"
#include "types.h"

struct UnkInner {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkNode {
    u8 pad_0x0[8];
    UnkInner* inner_0x8;
};

struct UnkHashed {
    u8 pad_0x0[0xe0];
    u32 field_0xe0;
    u8 pad_0xe4[4];
    UnkNode* node_0xe8;
    UnkNode* node_0xf0;
};

u32 fn_710009d110(UnkHashed* self);

s32 fn_7100098ae8(UnkHashed* self) {
    UnkScopedHashStream s(0x16, nullptr);
    s.Put(fn_710009d110(self));
    s.Put(self->node_0xf0->inner_0x8->field_0x30);
    s.Put(self->field_0xe0);
    return s.Finish();
}
