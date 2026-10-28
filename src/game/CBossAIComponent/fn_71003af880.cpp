// Appends an element to an engine array of 0x20-byte records (two string ids and 12 bytes of
// plain data), moving it in; growing moves the old records over and destroys them.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkRecord_71003af880 {
    base::UnkStrIdMember field_0x0;
    base::UnkStrIdMember field_0x8;
    u64 field_0x10;
    u32 field_0x18;
};

void fn_71003af880(base::UnkVector<UnkRecord_71003af880>* self, UnkRecord_71003af880* value) {
    self->EmplaceBack(static_cast<UnkRecord_71003af880&&>(*value));
}
