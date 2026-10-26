// The constructor of the 0x70-byte registry (unit 63, engine/base).
#include <string.h>

#include "base/UnkRegistry.h"

extern char g_unk_7101a1afb0[];  // the registry's vtable (GOT 0x7101cbc3d8)

void fn_7100087388(UnkRegistry_0x70* self) {
    self->vtable = g_unk_7101a1afb0 + 0x10;
    base::UnkAllocator* alloc = fn_7100083990();
    typedef u64 __attribute__((may_alias)) word;
    char* base = reinterpret_cast<char*>(self);
    self->flags = 0xe;
    *reinterpret_cast<word*>(base + 0x24) = 0xa00000000;  // capacity 0, step 10
    self->allocator = alloc;
    *reinterpret_cast<word*>(base + 0x14) = 0;
    *reinterpret_cast<word*>(base + 0x1c) = 0;
    self->entries.field_0x14 = true;
    self->entries.flags = 0xe;
    self->entries.allocator = fn_7100083990();
    self->field_0x48 = 0;
    self->field_0x40 = 0;
    if (self->entries.count < 11) {
        if (self->entries.capacity < 11) {
            void** p = static_cast<void**>(self->entries.allocator->vf_0x40(0x58, 8, self->entries.flags));
            if (self->entries.data) {
                memcpy(p, self->entries.data, (u64)self->entries.count << 3);
                self->entries.allocator->vf_0x48(self->entries.data);
            }
            self->entries.capacity = 11;
            self->entries.data = p;
        }
        memset(self->entries.data + self->entries.count, 0, (u64)(11 - self->entries.count) << 3);
    } else if (self->entries.count == 11) {
        goto tail;
    }
    self->entries.count = 11;
tail:
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->field_0x50 = 0;
    self->field_0x58 = 0;
}
