// Constructor of an unidentified class: runs the base constructor, sets the vtable, copies one
// global string id into nine id members, sets a flag and two words (0xdac and 5).
#include <new>

#include "types.h"

// A pooled string's record with a plain reference count (CStrId.h makes the count a union
// member, which would force reloads the original doesn't have).
struct UnkStringData_71008bca30 {
    u8 unk_0x0[0x8];
    s32 ref_count;
};
struct UnkStringPool_71008bca30 {
    u8 unk_0x0[0x10];
    UnkStringData_71008bca30 empty;
};
extern UnkStringPool_71008bca30* g_unk_7101d47108;  // the string pool (reached through the GOT)

struct UnkStrId_71008bca30 {
    UnkStringData_71008bca30* data;
    UnkStrId_71008bca30(const UnkStrId_71008bca30& o) : data(o.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
};
extern UnkStrId_71008bca30 unk_7101cba090;  // a shared id (reached through the GOT)

extern char unk_7101cf99d0[];  // vtable (reached through the GOT)

struct UnkStruct_71008bca30 {
    char* vtable;
    u8 unk_0x8[0x10];
    u32 field_0x18;
    u32 field_0x1c;
    u8 unk_0x20[0x38];
    UnkStrId_71008bca30 ids[9];
    bool field_0xa0;
};

void fn_71011acd10(UnkStruct_71008bca30* self);  // base constructor

void fn_71008bca30(UnkStruct_71008bca30* self) {
    fn_71011acd10(self);
    self->vtable = unk_7101cf99d0 + 0x10;
    for (s32 i = 0; i < 9; ++i)
        new (&self->ids[i]) UnkStrId_71008bca30(unk_7101cba090);
    self->field_0xa0 = true;
    self->field_0x18 = 0xdac;
    self->field_0x1c = 5;
}
