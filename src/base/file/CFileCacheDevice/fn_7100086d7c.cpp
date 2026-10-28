// Destructor of a class (vtable unk_7101a1aeb0) with a string id at 8, a member at 0x18
// (destroyed by fn_71000853b8) and one at 0xb0 (cleared by fn_7100085d8c, then destroyed by
// fn_7100085d68); it has no base destructor. Called only through the vtable.

#include "base/UnkDestructors.h"
#include "types.h"

extern char unk_7101a1aeb0[];
void fn_7100085d8c(void* member);
void fn_7100085d68(void* member);
void fn_71000853b8(void* member);

struct UnkObject_7100086d7c {
    char* vtable;
    base::UnkStrIdMember id;
    u64 field_0x10;
    u8 member_0x18[0x98];
    u8 member_0xb0[0x8];
};

void fn_7100086d7c(UnkObject_7100086d7c* self) {
    self->vtable = unk_7101a1aeb0 + 0x10;
    fn_7100085d8c(self->member_0xb0);
    fn_7100085d68(self->member_0xb0);
    fn_71000853b8(self->member_0x18);
    using Id = base::UnkStrIdMember;
    self->id.~Id();
}
