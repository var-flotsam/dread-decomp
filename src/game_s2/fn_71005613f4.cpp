// Constructor of an object holding two records of the same layout (a vtable pair, cleared
// words, defaults 10, 1 and 15, and a pointer from fn_7100083990), the second one without
// its first vtable.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a48248[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];

struct UnkPair {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkRecord {
    void** field_0x0;
    void** field_0x8;
    UnkPair field_0x10;
    u32 field_0x20;
    u8 field_0x24;
    u8 pad_0x25[3];
    u32 field_0x28;
    u8 pad_0x2c[4];
    void* field_0x30;
    u32 field_0x38;
};

struct UnkSelf {
    UnkRecord field_0x0;
    UnkRecord field_0x40;
    u64 field_0x80;
};

void fn_71005613f4(UnkSelf* self) {
    self->field_0x0.field_0x0 = g_unk_7101a48248 + 2;
    self->field_0x0.field_0x8 = g_unk_7101a20998 + 2;
    self->field_0x0.field_0x10.field_0x0 = 0;
    self->field_0x0.field_0x10.field_0x8 = 0;
    self->field_0x0.field_0x20 = 10;
    self->field_0x0.field_0x24 = 1;
    self->field_0x0.field_0x28 = 15;
    void* first = fn_7100083990();
    void** final_vtable = g_unk_7101a20950 + 2;
    self->field_0x0.field_0x8 = final_vtable;
    self->field_0x0.field_0x30 = first;
    self->field_0x0.field_0x38 = 0;
    self->field_0x40.field_0x0 = nullptr;
    self->field_0x40.field_0x8 = g_unk_7101a20998 + 2;
    self->field_0x40.field_0x10.field_0x0 = 0;
    self->field_0x40.field_0x10.field_0x8 = 0;
    self->field_0x40.field_0x20 = 10;
    self->field_0x40.field_0x24 = 1;
    self->field_0x40.field_0x28 = 15;
    void* second = fn_7100083990();
    self->field_0x40.field_0x30 = second;
    self->field_0x40.field_0x38 = 0;
    self->field_0x40.field_0x8 = final_vtable;
    self->field_0x80 = 0;
}
