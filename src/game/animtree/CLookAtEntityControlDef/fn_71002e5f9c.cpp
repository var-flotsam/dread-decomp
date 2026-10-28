// Constructor of a three-level object (each level sets its own vtable): copies the default string
// id into the one at 0x8 and makes the ids at 0x10 and 0x40 the empty string (creating the pool
// first when there is none).
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkNode {
    void** vtable;
    CStrId field_0x8;
    UnkStringData* field_0x10;
    u8 pad_0x18[0x28];
    UnkStringData* field_0x40;
};

extern void* g_unk_7101a21718[];
extern void* g_unk_7101a21af8[];
extern void* g_unk_7101a21ef0[];

void fn_71002e5f9c(UnkNode* self) {
    self->vtable = g_unk_7101a21718 + 2;
    new (&self->field_0x8) CStrId(g_unk_7101d470e0);
    self->vtable = g_unk_7101a21af8 + 2;
    self->field_0x10 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x10 = &g_unk_7101d47108->empty;
    self->vtable = g_unk_7101a21ef0 + 2;
    self->field_0x40 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x40 = &g_unk_7101d47108->empty;
}
