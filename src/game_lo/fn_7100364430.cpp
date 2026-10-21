// Destructor of an object holding an array of owned objects and five string ids: deletes the
// owned objects, releases the string ids, then frees the array.
#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"

struct UnkTag_fn_7100364430;

// The class's vtable (reached through the GOT).
extern char g_unk_7101cd1000[];

struct UnkObject_7100364430 {
    char* vtable;
    UnkCloneOwnerVec<UnkTag_fn_7100364430> items;  // freed last
    base::UnkStrIdMember field_0x30;
    base::UnkStrIdMember field_0x38;
    u64 field_0x40[8];
    base::UnkStrIdMember field_0x80;
    base::UnkStrIdMember field_0x88;
    u64 field_0x90[7];
    base::UnkStrIdMember field_0xc8;

    __attribute__((always_inline)) ~UnkObject_7100364430() {
        vtable = g_unk_7101cd1000 + 0x10;
        items.v.Clear();
    }
};

void fn_7100364430(UnkObject_7100364430* self) {
    self->~UnkObject_7100364430();
}
