// Destroys an object's inline array of 12-byte elements (each with an out-of-line destructor),
// clears the count, then releases the refcounted pointer at +0x58.
#include "base/UnkDestructors.h"
#include "types.h"

using base::UnkRefPtrMember;

struct UnkElem_7100286624 {
    u8 unk_0x0[0xc];
};
void fn_710028760c(UnkElem_7100286624* self);  // the element's destructor (empty)

struct UnkStruct_7100286624 {
    u8 unk_0x0[0x58];
    UnkRefPtrMember field_0x58;
    u8 unk_0x60[0x90 - 0x60];
    u32 count;
    UnkElem_7100286624 elems[1];
};

void fn_7100286624(UnkStruct_7100286624* self) {
    for (u64 i = 0; i < self->count; i++)
        fn_710028760c(&self->elems[i]);
    self->count = 0;
    self->field_0x58.~UnkRefPtrMember();
}
