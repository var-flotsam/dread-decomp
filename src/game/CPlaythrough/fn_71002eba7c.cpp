// Constructor of an object that holds two copies of the default string id (+0x58, +0x60), a
// copy of the default file path id (+0x68), an array of another layout (+0x70: no flags, a mode
// of 2 and its own allocator), an empty string id at +0xa8 (the pool created if need be) and a
// growable array of ids (+0xb0, flags 0xf). The base's constructor (fn_7100016ad8) runs first.
#include <new>

#include "base/global/CRntVector.h"
#include "base/core/CAsset.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CPlaythrough.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// Two zeroed blocks of two words, the step and mode, and an allocator.
struct UnkModeArray {
    u64 field_0x0[2] = {};
    u64 field_0x10[2] = {};
    u32 step = 10;
    u32 mode = 2;
    base::IAllocator* allocator = GetDefaultAllocator();
    u16 field_0x28 = 0;
    u8 pad_0x2a;
    u8 field_0x2b = 0;
    u32 pad_0x2c;
};

struct UnkIdHolder {
    void** vtable;
    u8 pad_0x8[0x50];
    CStrId field_0x58;
    CStrId field_0x60;
    CFilePathStrId field_0x68;
    UnkModeArray field_0x70;
    CStrId field_0xa8;
    base::global::CRntVector<void*> field_0xb0;
    u64 field_0xd8;
    u64 field_0xe0[2];
    u64 field_0xf0;
    u32 field_0xf8;
    u32 pad_0xfc;
    u64 field_0x100[4];
    u64 field_0x120;
};
static_assert(sizeof(UnkIdHolder) == 0x128, "UnkIdHolder layout");

void fn_71002eba7c(UnkIdHolder* self) {
    fn_7100016ad8(self);
    self->vtable = g_unk_7101a22200 + 2;
    new (&self->field_0x58) CStrId(base::global::CStrId::Empty());
    new (&self->field_0x60) CStrId(base::global::CStrId::Empty());
    new (&self->field_0x68) CFilePathStrId(g_unk_7101d470e8);
    new (&self->field_0x70) UnkModeArray;
    self->field_0xa8.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0xa8.data = &g_unk_7101d47108->empty;
    new (&self->field_0xb0) base::global::CRntVector<void*>(0xf);
    self->field_0xd8 = 0;
    self->field_0xf0 = 0;
    self->field_0xf8 = 0;
    self->field_0x120 = 0;
    self->field_0xe0[0] = 0;
    self->field_0xe0[1] = 0;
    self->field_0x100[0] = 0;
    self->field_0x100[1] = 0;
    self->field_0x100[2] = 0;
    self->field_0x100[3] = 0;
}
