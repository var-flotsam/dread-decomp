// Unit 207 (fn_710016b0ec): entry `index`'s object, when the registry g_unk_716e34c2e0 still
// holds it, after setting its flag and the flag of its name's hash when the entry has a name.
// fn_710016b194: entry `index` of a list of { object, name } pairs is handed to fn_710019962c when
// the registry g_unk_716e34c2e0 still holds the object and the entry has a name. Names and
// layouts are placeholders inferred from the code.

#include "base/global/CStrId.h"
#include "game/UnkTU218.h"
#include "types.h"

struct UnkKeySetOwner;
bool fn_710019e4fc(UnkKeySetOwner* self, u64 key);
void fn_710019962c(void* object);
void fn_71001995e8(void* object, bool value);
extern void* g_unk_716e34c2e0;

struct UnkNamedEntry_710016b194 {
    u64 object;
    base::global::CStrId name;
};

struct UnkNamedEntries_710016b194 {
    u8 pad_0x0[0x10];
    UnkNamedEntry_710016b194* entries;
    u32 count;
};

void fn_710016b194(UnkNamedEntries_710016b194* self, u32 index) {
    if (index < self->count) {
        UnkNamedEntry_710016b194* entries = self->entries;
        u64 object = entries[index].object;
        if (fn_710019e4fc(static_cast<UnkKeySetOwner*>(g_unk_716e34c2e0), object) &&
            entries[index].name.data != &g_unk_7101d47108->empty)
            fn_710019962c(reinterpret_cast<void*>(object));
    }
}

void* fn_710016b0ec(UnkNamedEntries_710016b194* self, u32 index) {
    if (index < self->count) {
        UnkNamedEntry_710016b194* entries = self->entries;
        u64 object = entries[index].object;
        if (fn_710019e4fc(static_cast<UnkKeySetOwner*>(g_unk_716e34c2e0), object)) {
            base::global::CStrId& name = entries[index].name;
            if (name.data != &g_unk_7101d47108->empty) {
                fn_71001995e8(reinterpret_cast<void*>(object), true);
                u64 hash = *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(name.data) + 0x30);
                fn_7100199420(reinterpret_cast<void*>(object), &hash, true);
            }
            return reinterpret_cast<void*>(object);
        }
    }
    return nullptr;
}
