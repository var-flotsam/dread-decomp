// Constructor of an unidentified class: runs the base constructor, sets the vtable, copies one
// global string id into nine id members, sets a flag and two words (0xdac and 5).
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"  // g_unk_7101d470e0, the shared id
#include "game/CCharClassComponent.h"
#include "game/CCharClassLifeComponent.h"
#include "types.h"

struct UnkStrId_71008bca30 {
    base::global::UnkStringData* data;
    // CStrId's copy constructor: retains unless the string is the pool's empty one.
    explicit UnkStrId_71008bca30(base::global::UnkStringData* d)
        : data(d) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
};

extern char g_unk_7101a7deb0[];  // vtable (reached through the GOT)

struct UnkStruct_71008bca30 {
    char* vtable;
    u8 unk_0x8[0x10];
    u32 field_0x18;
    u32 field_0x1c;
    u8 unk_0x20[0x38];
    UnkStrId_71008bca30 ids[9];
    bool field_0xa0;
};

void fn_71008bca30(UnkStruct_71008bca30* self) {
    fn_71011acd10(self);
    self->vtable = g_unk_7101a7deb0 + 0x10;
    for (s32 i = 0; i < 9; ++i)
        new (&self->ids[i]) UnkStrId_71008bca30(g_unk_7101d470e0.data);
    self->field_0xa0 = true;
    self->field_0x18 = 0xdac;
    self->field_0x1c = 5;
}
