// Constructor of an unidentified polymorphic object: sets its vtable, an empty container using the
// default allocator, several empty string ids and a block of defaults (-1.0f sentinels, flags).
#include <new>

#include "game/UnkVtable_7101a24158.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_71003641fc {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    base::UnkAllocator* field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
    f32 field_0x40;
    f32 field_0x44;
    u32 field_0x48;
    bool field_0x4c;
    f32 field_0x50;
    bool field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
    u64 field_0x60;
    u64 field_0x68;
    f32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    bool field_0x7c;
    u16 field_0x7e;
    CStrId field_0x80;
    CStrId field_0x88;
    f32 field_0x90;
    f32 field_0x94;
    f32 field_0x98;
    f32 field_0x9c;
    f32 field_0xa0;
    s32 field_0xa4;
    s32 field_0xa8;
    bool field_0xac;
    bool field_0xad;
    u32 field_0xb0;
    bool field_0xb4;
    f32 field_0xb8;
    f32 field_0xbc;
    bool field_0xc0;
    CStrId field_0xc8;
    f32 field_0xd0;
    f32 field_0xd4;
    bool field_0xd8;
    f32 field_0xdc;
    bool field_0xe0;
    f32 field_0xe4;
    u32 field_0xe8;
};

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_71003641fc(UnkStruct_71003641fc* self) {
    self->vtable = g_unk_7101a24158 + 0x10;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    self->field_0x18 = 10;
    self->field_0x1c = true;
    self->field_0x20 = 4;
    self->field_0x28 = fn_7100083990();
    new (&self->field_0x30) CStrId("", true);
    new (&self->field_0x38) CStrId("", true);
    self->field_0x40 = -1.0f;
    self->field_0x44 = -1.0f;
    self->field_0x48 = 0;
    self->field_0x4c = true;
    self->field_0x50 = 0.4f;
    self->field_0x54 = false;
    self->field_0x58 = 0;
    self->field_0x5c = 3;
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->field_0x70 = -1.0f;
    self->field_0x74 = 0;
    self->field_0x78 = 1;
    self->field_0x7c = true;
    self->field_0x7e = 0;
    InitEmptyStrId(&self->field_0x80);
    InitEmptyStrId(&self->field_0x88);
    self->field_0x90 = -1.0f;
    self->field_0x94 = -1.0f;
    self->field_0x98 = -1.0f;
    self->field_0x9c = -1.0f;
    self->field_0xa0 = -1.0f;
    self->field_0xa4 = -1;
    self->field_0xa8 = -1;
    self->field_0xac = true;
    self->field_0xad = false;
    self->field_0xb0 = 0;
    self->field_0xb4 = false;
    self->field_0xb8 = -1.0f;
    self->field_0xbc = -1.0f;
    self->field_0xc0 = true;
    new (&self->field_0xc8) CStrId("", true);
    self->field_0xd0 = -1.0f;
    self->field_0xd4 = -1.0f;
    self->field_0xd8 = true;
    self->field_0xdc = 0.0f;
    self->field_0xe0 = true;
    self->field_0xe4 = -1.0f;
    self->field_0xe8 = 0;
}
