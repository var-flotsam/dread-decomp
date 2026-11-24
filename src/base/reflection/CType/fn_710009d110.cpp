// State checksum of an object: tag 0x14, a 64-bit value from its first reference, the same
// value from an optional second one (-1 if absent) and the hash of an optional third (-1).
#include "base/UnkHashStream.h"
#include "base/UnkInner.h"
#include "base/reflection/CType.h"
#include "game/UnkTU70.h"

struct UnkNode_09d110 {
    u8 pad_0x0[8];
    UnkInner* inner_0x8;
};

struct UnkHashed_710009d110 {
    u8 pad_0x0[8];
    UnkInner* inner_0x8;
    u8 pad_0x10[0x68];
    UnkNode_09d110* node_0x78;
    void* ref_0x80;
};

u32 fn_710009d110(UnkHashed_710009d110* self) {
    UnkScopedHashStream s(0x16, nullptr);
    s.Begin(0x14);
    s.Put(self->inner_0x8->field_0x30);
    s.Put(self->node_0x78 ? self->node_0x78->inner_0x8->field_0x30 : (u64)-1);
    s.Put(self->ref_0x80 ? HashFlaggedAttributeKeysAndValues(self->ref_0x80) : (u32)-1);
    return s.Finish();
}
