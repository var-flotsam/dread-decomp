// Destructor of a class (vtable unk_7101a1f000) that marks the word at 0x100 as dead
// (0xdeaddead), destroys a member at 0x80 (fn_7100216934), drops a counted object at 0x78 (count
// at +0x54, freed by fn_710001737c) and releases a string id at 0x10; it has no base destructor.
// fn_710019e82c is a smaller relative.

#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "types.h"

extern char unk_7101a1f000[];
void fn_7100216934(char* p);

struct UnkObject_71001f3438 {
    char* vtable;
    u64 field_0x8;
    base::UnkStrIdMember id;
    u8 pad_0x18[0x60];
    UnkCountedResource* counted;
    char member_0x80[0x80];
    u32 field_0x100;
};

void fn_71001f3438(UnkObject_71001f3438* self) {
    self->vtable = unk_7101a1f000 + 0x10;
    self->field_0x100 = 0xdeaddead;
    fn_7100216934(self->member_0x80);
    if (UnkCountedResource* counted = self->counted) {
        if (--counted->ref_count == 0)
            fn_710001737c(counted);
    }
    using Id = base::UnkStrIdMember;
    self->id.~Id();
}
