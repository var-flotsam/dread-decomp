// Unit 219 (tools/tu_map.py): the constructor of a list of 16-byte entries owned by another object
// (+0x28): growth step 10, allocator flags 0x32, room for 5 entries reserved up front and three flags set.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkEntry16_b0242 {
    u64 a;
    u64 b;
    UnkEntry16_b0242(UnkEntry16_b0242&& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
};

struct UnkOwnerList_b0242 {
    base::UnkVector<UnkEntry16_b0242> entries;
    void* owner;  // 0x28
    bool field_0x30;
    bool field_0x31;
    bool field_0x32;
};
static_assert(__builtin_offsetof(UnkOwnerList_b0242, owner) == 0x28, "layout");

void fn_7100199fcc(UnkOwnerList_b0242* self, void* owner) {
    new (&self->entries) base::UnkVector<UnkEntry16_b0242>(0x32);
    self->entries.Reserve(5);
    self->owner = owner;
    self->field_0x32 = true;
    self->field_0x30 = true;
    self->field_0x31 = true;
}
