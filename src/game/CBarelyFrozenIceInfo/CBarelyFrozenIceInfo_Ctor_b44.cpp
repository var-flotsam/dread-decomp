// CBarelyFrozenIceInfo (tools/tu_map.py; the name from reflection): the constructor. The base
// constructor fn_71007ee4c8, the vtable, cleared fields, two copies of the empty string id
// (g_unk_7101d470e0) at +0xb0 and +0xb8, an empty array at +0xc8 (capacity 10, flags 0xf, the
// default allocator), then the vtable's slot at +0x18 on itself. Names and layout are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkFrozenIceBase_b44a2;
void fn_71007ee4c8(UnkFrozenIceBase_b44a2* self);
extern void* g_unk_7101a73e38[];  // the class's vtable

typedef u64 UnkU64Align4_b44a2 __attribute__((aligned(4)));

using UnkIdData_b44a2 = base::global::UnkStringData;

struct UnkFrozenIceInfo_b44a2 {
    void** vtable;
    u8 pad_0x8[0x98];
    u32 field_0xa0;
    UnkU64Align4_b44a2 field_0xa4;
    u32 pad_0xac;
    UnkIdData_b44a2* field_0xb0;  // CStrId
    UnkIdData_b44a2* field_0xb8;  // CStrId
    bool field_0xc0;
    u8 pad_0xc1[7];
    u64 field_0xc8;
    u64 field_0xd0;
    u32 field_0xd8;
    bool field_0xdc;
    u32 field_0xe0;
    base::UnkAllocator* field_0xe8;
};

void fn_71007f3100(UnkFrozenIceInfo_b44a2* self) {
    fn_71007ee4c8(reinterpret_cast<UnkFrozenIceBase_b44a2*>(self));
    self->vtable = g_unk_7101a73e38 + 2;
    self->field_0xa0 = 0;
    self->field_0xa4 = 0;
    // Both copies of the empty id: the id and the pool's empty one are read once.
    UnkIdData_b44a2* id = g_unk_7101d470e0.data;
    self->field_0xb0 = id;
    bool counted = &g_unk_7101d47108->empty != id;
    if (counted)
        ++id->ref_count;
    self->field_0xb8 = id;
    if (counted)
        ++id->ref_count;
    self->field_0xc0 = false;
    self->field_0xc8 = 0;
    self->field_0xd0 = 0;
    self->field_0xd8 = 10;
    self->field_0xdc = true;
    self->field_0xe0 = 0xf;
    self->field_0xe8 = fn_7100083990();
    reinterpret_cast<void (*)(UnkFrozenIceInfo_b44a2*)>(self->vtable[3])(self);
}

// A record of two copies of the empty string id (+0x8, +0x10) and a scale of 1 (+0x18): its
// constructor, with the ids copied as in fn_71007f3100.
extern void* g_unk_7101a73e10[];  // the record's vtable

struct UnkFrozenIceRecord_b44a2 {
    void** vtable;
    UnkIdData_b44a2* field_0x8;   // CStrId
    UnkIdData_b44a2* field_0x10;  // CStrId
    f32 field_0x18;
};

void fn_71007f1e6c(UnkFrozenIceRecord_b44a2* self) {
    self->vtable = g_unk_7101a73e10 + 2;
    UnkIdData_b44a2* id = g_unk_7101d470e0.data;
    self->field_0x8 = id;
    bool counted = &g_unk_7101d47108->empty != id;
    if (counted)
        ++id->ref_count;
    self->field_0x10 = id;
    if (counted)
        ++id->ref_count;
    self->field_0x18 = 1.0f;
}
