// Constructor of an unidentified polymorphic object: sets its vtable, clears a run of fields
// (one sentinel of -1), copies a global string id, sets two empty string ids, then finishes
// in fn_71008c0e30.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern void* unk_7101a7e178[];  // vtable (reached through the GOT)
extern CStrId g_unk_7101d470e0;  // a global string id

struct UnkStruct_71008c0c74 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u8 pad_0x18[0x20];
    u64 field_0x38;
    u64 field_0x40;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u8 pad_0x5c[0xc];
    s32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;
    u32 field_0x8c;
    u32 field_0x90;
    u32 field_0x94;
    u32 field_0x98;
    CStrId field_0xa0;
    CStrId field_0xa8;
    u64 field_0xb0;
    CStrId field_0xb8;
    u8 pad_0xc0[0x18];
    u64 field_0xd8;
    u64 field_0xe0;
    u64 field_0xe8;
};

void fn_71008c0e30(UnkStruct_71008c0c74* self);

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_71008c0c74(UnkStruct_71008c0c74* self) {
    self->vtable = &unk_7101a7e178[2];
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    self->field_0x38 = 0;
    self->field_0x40 = 0;
    self->field_0x4c = 0;
    self->field_0x50 = 0;
    self->field_0x54 = 0;
    self->field_0x58 = 0;
    self->field_0x68 = -1;
    self->field_0x6c = 0;
    self->field_0x70 = 0;
    self->field_0x74 = 0;
    self->field_0x78 = 0;
    self->field_0x7c = 0;
    self->field_0x80 = 0;
    self->field_0x84 = 0;
    self->field_0x88 = 0;
    self->field_0x8c = 0;
    self->field_0x90 = 0;
    self->field_0x94 = 0;
    new (&self->field_0xa0) CStrId(g_unk_7101d470e0);
    InitEmptyStrId(&self->field_0xa8);
    InitEmptyStrId(&self->field_0xb8);
    self->field_0xd8 = 0;
    self->field_0xe0 = 0;
    self->field_0xe8 = 0;
    fn_71008c0e30(self);
}
