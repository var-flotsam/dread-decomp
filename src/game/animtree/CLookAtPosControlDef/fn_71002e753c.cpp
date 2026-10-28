// Constructor of a three-level object (each level sets its own vtable): clears the words at
// 0x10..0x3f, copies the default string id into the one at 0x8 and makes the id at 0x10 the empty
// string (creating the pool first when there is none).
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkPair {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkNode {
    void** vtable;
    CStrId field_0x8;
    UnkPair field_0x10;
    UnkPair field_0x20;
    UnkPair field_0x30;
};

extern void* g_unk_7101a21718[];
extern void* g_unk_7101a21af8[];
extern void* g_unk_7101a21fc8[];

void fn_71002e753c(UnkNode* self) {
    self->field_0x30 = UnkPair{0, 0};
    self->field_0x20 = UnkPair{0, 0};
    self->field_0x10 = UnkPair{0, 0};
    self->vtable = g_unk_7101a21718 + 2;
    new (&self->field_0x8) CStrId(g_unk_7101d470e0);
    self->vtable = g_unk_7101a21af8 + 2;
    self->field_0x10.field_0x0 = 0;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x10.field_0x0 = reinterpret_cast<u64>(&g_unk_7101d47108->empty);
    self->vtable = g_unk_7101a21fc8 + 2;
}
