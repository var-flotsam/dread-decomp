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

// ---- fn_7100087850: whether the registry has an entry for a path ----

// A word set to zero on first use (a header static: guard and word reached through the GOT).
struct UnkRegistryZeroWord {
    u32 value;
    UnkRegistryZeroWord() { value = 0; }
};
inline UnkRegistryZeroWord& UnkRegistryDefaultWord() {
    static UnkRegistryZeroWord s_word;
    return s_word;
}

// What the registry's lookup (vf_0x20) fills in: whether the key was found, a word, and more the
// caller leaves unset (0x30 bytes in all).
struct UnkRegistryLookup {
    bool found;
    u32 field_0x4;
    u8 unk_0x8[0x28];
};

struct UnkRegistryVtable {
    void* slots[4];
    void (*lookup)(UnkRegistry_0x70* self, const UnkStrPair_0x10* key, UnkRegistryLookup* out, const u32* in);
};

bool fn_7100087850(UnkRegistry_0x70* self, const UnkStrPair_0x10* key) {
    UnkRegistryLookup result;
    result.found = false;
    result.field_0x4 = 0;
    alignas(8) u32 in = UnkRegistryDefaultWord().value;
    static_cast<UnkRegistryVtable*>(self->vtable)->lookup(self, key, &result, &in);
    return result.found;
}
