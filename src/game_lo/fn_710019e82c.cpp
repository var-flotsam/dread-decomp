// The destructor of a class with a counted object at +0x10 (count at +0x54, freed by
// fn_710001737c when it drops to zero) and a string id at +8; it has no base destructor.
#include "base/UnkDestructors.h"
#include "types.h"

using base::UnkStrIdMember;

void fn_710001737c(char* p);

extern char g_unk_7101a1e2c8[];  // vtable (reached through the GOT)

struct UnkStruct_710019e82c {
    char* vtable;
    base::UnkStrIdMember field_0x8;
    char* field_0x10;
};

void fn_710019e82c(UnkStruct_710019e82c* self) {
    self->vtable = g_unk_7101a1e2c8 + 0x10;
    if (char* counted = self->field_0x10) {
        if (--*reinterpret_cast<s32*>(counted + 0x54) == 0)
            fn_710001737c(counted);
    }
    self->field_0x8.~UnkStrIdMember();
}
