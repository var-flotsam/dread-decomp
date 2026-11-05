// animtree::CLookAtPosControlDef's copy assignment (fn_0x48 of its reflected class,
// src/reflection/animtree_CLookAtPosControlDef.cpp): the two string ids of the CBoneControlDef base (+0x8, +0x10)
// with CStrId's copy assignment, then the plain values at +0x18 (a 16-byte pair, then the 0x14 bytes from +0x28
// copied as one block). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkLookAtPosPair_A2_b76 {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkLookAtPosControlDef_A2_b76 {
    void** vtable;
    CStrId name;  // 0x8
    CStrId bone;  // 0x10
    UnkLookAtPosPair_A2_b76 field_0x18;
    UnkLookAtPosPair_A2_b76 field_0x28;
    u32 field_0x38;
};

void* fn_71002e7788(void* dst, const void* src) {
    UnkLookAtPosControlDef_A2_b76* self = static_cast<UnkLookAtPosControlDef_A2_b76*>(dst);
    const UnkLookAtPosControlDef_A2_b76* other = static_cast<const UnkLookAtPosControlDef_A2_b76*>(src);
    if (self != other) {
        self->name = other->name;
        self->bone = other->bone;
        self->field_0x18 = other->field_0x18;
        __builtin_memcpy(&self->field_0x28, &other->field_0x28, 0x14);
    }
    return self;
}
