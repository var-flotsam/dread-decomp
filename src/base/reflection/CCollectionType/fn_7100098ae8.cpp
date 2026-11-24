// Computes a hash over an object's state: opens a stream (kind 0x16), appends its id, a
// 64-bit value reached through a pointer chain and the u32 at +0xe0, and returns the digest of the buffer.
#include "base/UnkHashStream.h"
#include "base/UnkHashed.h"
#include "base/UnkInner.h"
#include "base/reflection/CType.h"
#include "types.h"

struct UnkNode {
    u8 pad_0x0[8];
    UnkInner* inner_0x8;
};

s32 fn_7100098ae8(UnkHashed* self) {
    UnkScopedHashStream s(0x16, nullptr);
    s.Put(CType_ComputeBaseHash(self));
    s.Put(self->node_0xf0->inner_0x8->field_0x30);
    s.Put(self->field_0xe0);
    return s.Finish();
}
