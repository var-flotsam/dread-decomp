// Constructor of a component base (shared by 25 derived constructors): runs the base
// constructor, sets the vtable, copies the default id and default-initialises the rest.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkComponent_0x138 {
    void** vtable;
    u8 unk_0x8[0xe8];
    UnkStringData* field_0xf0;  // CStrId
    bool field_0xf8;
    u32 field_0xfc;
    bool field_0x100;
    UnkStringData* field_0x108;  // CStrId
    UnkStringData* field_0x110;  // CStrId
    bool field_0x118;
    bool field_0x119;
    u8 unk_0x11a[0xe];
    bool field_0x128;
    u8 unk_0x129[0x7];
    u16 field_0x130;
};

extern void* g_unk_7101a96758[];
void fn_7100ab38a0(UnkComponent_0x138* self);

// CStrId's default constructor: the pool's empty string, creating the pool if needed.
static inline void InitEmptyId(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_7100ac14f8(UnkComponent_0x138* self) {
    fn_7100ab38a0(self);
    self->vtable = &g_unk_7101a96758[2];
    UnkStringData* id = g_unk_7101d470e0.data;
    self->field_0xf0 = id;
    if (&g_unk_7101d47108->empty != id)
        ++*reinterpret_cast<s32*>(reinterpret_cast<char*>(id) + 8);
    self->field_0xf8 = false;
    self->field_0xfc = 0;
    self->field_0x100 = true;
    InitEmptyId(&self->field_0x108);
    InitEmptyId(&self->field_0x110);
    self->field_0x118 = true;
    self->field_0x119 = false;
    self->field_0x128 = true;
    self->field_0x130 = 0;
}
