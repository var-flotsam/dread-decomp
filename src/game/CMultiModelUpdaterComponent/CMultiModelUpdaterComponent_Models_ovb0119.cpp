// CMultiModelUpdaterComponent's unit (tools/tu_map.py unit 1426): fn_7100938ebc selects the model
// named by a string id, as the reflected setter fn_7100939440
// (CMultiModelUpdaterComponent_ReflSet_A5_b67.cpp) does inline: when it differs from the current one
// (+0x210), the model map (entries of 0x20 at +0x1e8, keys at +0x1f0, count at +0x1f8) has it and
// fn_7100938fcc accepts the entry, it takes the name; it returns whether it did. Names are placeholders.
#include "base/global/CStrId.h"
#include "types.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkModelEntry_A5_b67;
struct UnkMultiModel_A5_b67;
bool fn_7100938fcc(UnkMultiModel_A5_b67* self, UnkModelEntry_A5_b67* entry);

struct UnkModelEntry_b0119 {
    u8 pad_0x0[0x20];
};

struct UnkMultiModel_b0119 {
    u8 pad_0x0[0x1e8];
    UnkModelEntry_b0119* values;
    const void** keys;
    u32 count;
    u8 pad_0x1fc[0x14];
    CStrId name;  // 0x210

    UnkModelEntry_b0119* Find(const void* const& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

bool fn_7100938ebc(UnkMultiModel_b0119* self, const CStrId& name) {
    if (self->name.data == name.data)
        return false;
    UnkModelEntry_b0119* it = self->Find(name.data);
    if (it == self->values + self->count)
        return false;
    if (!fn_7100938fcc(reinterpret_cast<UnkMultiModel_A5_b67*>(self), reinterpret_cast<UnkModelEntry_A5_b67*>(it)))
        return false;
    self->name = name;
    return true;
}

// CMultiModelUpdaterComponent's unit (tools/tu_map.py unit 1426): fn_7100939678 grows the model
// map's two arrays (0x20-byte entries, string-id keys) to a new capacity: each entry is
// copy-constructed into the new array (fn_7100939804) and destroyed (fn_7100938dec), each key moved;
// the old arrays are freed. Names and layouts are placeholders.
void fn_7100938dec(UnkStrIdPathResource* self);

struct UnkModelValue_b0119 {
    u8 data[0x20];
};
void fn_7100939804(UnkModelValue_b0119* self, const UnkModelValue_b0119* other);

// A string id key: moved bitwise, the old one left holding the empty id and then released in two
// steps (as CStrId::Reset).
struct UnkMovedId_b0119 {
    UnkStringData* data;
};

struct UnkModelMap_b0119 {
    UnkModelValue_b0119* values;
    UnkMovedId_b0119* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100939678(UnkModelMap_b0119* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    auto* values = static_cast<UnkModelValue_b0119*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkModelValue_b0119), 8, self->flags));
    auto* keys = static_cast<UnkMovedId_b0119*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkMovedId_b0119), 8, self->flags));
    if (self->values) {
        UnkModelValue_b0119* src = self->values;
        UnkModelValue_b0119* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            fn_7100939804(dst, src);
            fn_7100938dec(reinterpret_cast<UnkStrIdPathResource*>(src));
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        UnkMovedId_b0119* src = self->keys;
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++src) {
            __builtin_memcpy(&keys[j], src, sizeof(UnkMovedId_b0119));
            UnkStringPool* pool = g_unk_7101d47108;
            src->data = &pool->empty;
            if ((pool = g_unk_7101d47108)) {
                if (src->data != &pool->empty) {
                    --src->data->ref_count;
                    if (src->data->ref_count == 0) {
                        fn_71000803a4(pool, src->data);
                        pool = g_unk_7101d47108;
                    }
                }
                src->data = &pool->empty;
            }
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = capacity;
}
