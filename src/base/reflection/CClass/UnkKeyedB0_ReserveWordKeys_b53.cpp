// base::reflection::CClass's unit (unit 72 of tools/tu_map.py): the reserve of the keyed array of
// 0xb0-byte records with plain-word keys that fn_71000dc190 copies
// (src/base/reflection/UnkReflKeyedArrayCopy.cpp, whose types are repeated here), as fn_71000dcd2c
// (src/base/reflection/UnkKeyedB0_Reserve_b52.cpp) does for string-id keys: each record moved
// (fn_7100097d18) and the old one destroyed (fn_71000e70c4), the keys copied through the aliasing
// word type (the count and the key pointer are reloaded). Names are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU85.h"
#include "types.h"

struct __attribute__((may_alias)) UnkAliasWord_71000dc190 {
    u64 value;
};

struct UnkValue_71000dc190 {
    u8 data[0xb0];
};

using UnkKeyedArray_71000dc190 = base::global::CRntSmallDictionary<UnkAliasWord_71000dc190, UnkValue_71000dc190>;

struct UnkRecordB0_b44a1;  // the record as src/base/reflection/UnkReflKeyedB0_b44.cpp names it
// As src/game_s1/fn_71000dbc34.cpp declares it.
void fn_7100097d18(void* self, void* other);

void fn_7100097e4c(UnkKeyedArray_71000dc190* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkValue_71000dc190* values = static_cast<UnkValue_71000dc190*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkValue_71000dc190), 8, self->flags));
        UnkAliasWord_71000dc190* keys = static_cast<UnkAliasWord_71000dc190*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkAliasWord_71000dc190), 8, self->flags));
        if (self->values) {
            UnkValue_71000dc190* src = self->values;
            UnkValue_71000dc190* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                fn_7100097d18(dst, src);
                fn_71000e70c4(reinterpret_cast<UnkRecordB0_b44a1*>(src));
            }
            self->allocator->vf_0x48(self->values);
        }
        if (UnkAliasWord_71000dc190* old = self->keys) {
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j)
                keys[j] = old[j];
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}
