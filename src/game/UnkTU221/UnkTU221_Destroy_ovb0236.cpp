// Unit 221 (no reliable name): frees a table of entries. Each entry's block at +0x18 is released with delete[] and its
// object at +0x0 through the default allocator; then the entry array itself. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkEntry_b0236a {
    void* obj;  // 0x0
    u8 pad_0x8[0x10];
    u8* data;  // 0x18
    u8 pad_0x20[8];
};

struct UnkTable_b0236a {
    u8 pad_0x0[0x14];
    u32 count;  // 0x14
    UnkEntry_b0236a* entries;  // 0x18
};

void fn_710019adec(UnkTable_b0236a* self) {
    if (self->entries) {
        for (u32 i = 0; i < self->count; i++) {
            if (self->entries[i].data) {
                fn_7100083c30(self->entries[i].data);
                __builtin_memset(&self->entries[i].data, 0, 8);
            }
            if (self->entries[i].obj) {
                fn_7100083990()->vf_0x48(self->entries[i].obj);
                __builtin_memset(&self->entries[i].obj, 0, 8);
            }
        }
        if (self->entries)
            fn_7100083c30(self->entries);
        self->entries = nullptr;
    }
}
