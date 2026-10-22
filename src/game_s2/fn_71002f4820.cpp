// Constructor of an object that holds two string ids (left as the empty string, creating the pool
// first when there is none), an inline string at +0x18 whose storage is the sub-object at +0x48, a
// third empty-string id at +0xa0 and a growable array at +0xa8 (flags 0xf).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkStorage {
    void** vtable;
    u8 pad_0x8[0x40];
    u64 field_0x48;
    u32 field_0x50;
    u8 pad_0x54[4];
    UnkStringData* field_0x58;
    base::UnkVector<void*> field_0x60;
};

struct UnkNode {
    void** vtable;
    UnkStringData* field_0x8;
    UnkStringData* field_0x10;
    u8 field_0x18[0x30];
    UnkStorage field_0x48;
};

void fn_710007e6e4(void* string, void* storage);
extern void* g_unk_7101a22280[];
extern void* g_unk_7101a1b678[];

void fn_71002f4820(UnkNode* self) {
    self->vtable = g_unk_7101a22280 + 2;
    self->field_0x8 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x8 = &g_unk_7101d47108->empty;
    self->field_0x10 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x10 = &g_unk_7101d47108->empty;
    fn_710007e6e4(self->field_0x18, &self->field_0x48);
    self->field_0x48.vtable = g_unk_7101a1b678 + 2;
    self->field_0x48.field_0x48 = 0;
    self->field_0x48.field_0x50 = 0;
    self->field_0x48.field_0x58 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x48.field_0x58 = &g_unk_7101d47108->empty;
    new (&self->field_0x48.field_0x60) base::UnkVector<void*>(0xf);
}
