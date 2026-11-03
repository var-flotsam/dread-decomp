// base::tunable::CTunable's unit (unit 111): copying a registry of named objects
// (base/UnkNamedRegistry.h) and the two words and the pointer after it: its object pointers in one
// block, then its keys one by one. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkNamedRegistry.h"
#include "types.h"

// base::UnkNamedRegistry's fields, then the two words and the pointer.
struct UnkNamedRegistryCopy111 {
    base::UnkNamedObject** objects;
    base::UnkNamedEntry* entries;
    union {
        u32 count;
    };
    u32 capacity;
    u32 step;
    u32 field_0x1c;
    void* field_0x20;
};

void fn_7100138d0c(UnkNamedRegistryCopy111* self, const UnkNamedRegistryCopy111* other) {
    self->entries = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->objects = nullptr;
    self->step = other->step;
    self->field_0x1c = other->field_0x1c;
    self->field_0x20 = other->field_0x20;
    fn_710007547c(reinterpret_cast<base::UnkNamedRegistry*>(self), other->count);
    memcpy(self->objects, other->objects, (u64)other->count << 3);
    self->count = other->count;
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        fn_710007ea60(&self->entries[i], &other->entries[i]);
}
