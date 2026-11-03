// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): a reflected keyed array of
// 0x118-byte records by u32 key (values at +0, keys at +8, count at +0x10, allocator at +0x20), as
// fn_7100eaddf4 assigns into: its clear (CCollectionType +0x110), destructor (+0x40), Begin (+0x160)
// and End (+0x168). Shaped like src/base/reflection/UnkReflKeyedB0_b44.cpp. The records are
// destroyed by fn_7100ea7034. Names and layouts are placeholders.
#include "../../base/reflection/UnkTempIterValue_b44.h"
#include "base/UnkVector.h"
#include "types.h"

// As src/game/logic/collision/CCollider/fn_7100d3d424.cpp declares it.
struct UnkRecord118;
void fn_7100ea7034(UnkRecord118* self);

struct UnkRecord118_b45a3 {
    u64 words[0x118 / 8];

    ~UnkRecord118_b45a3() { fn_7100ea7034(reinterpret_cast<UnkRecord118*>(this)); }
};

struct UnkKeyed118Array_b45a3 {
    UnkRecord118_b45a3* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

__attribute__((always_inline)) static inline void Clear(UnkKeyed118Array_b45a3* self) {
    if (self->values)
        for (u32 i = 0; i < self->count; ++i)
            self->values[i].~UnkRecord118_b45a3();
    self->count = 0;
}

void fn_7100eadac4(UnkKeyed118Array_b45a3* self) { Clear(self); }

void fn_7100eadfb0(UnkKeyed118Array_b45a3* self) {
    Clear(self);
    if (self->keys)
        self->allocator->vf_0x48(self->keys);
    if (self->values)
        self->allocator->vf_0x48(self->values);
}

using UnkKeyed118Iter_b45a3 = UnkTempIterValue<UnkRecord118_b45a3, u32>;

void fn_7100eaeb3c(UnkKeyed118Array_b45a3* self, UnkKeyed118Iter_b45a3* out) {
    *out = UnkKeyed118Iter_b45a3(self->keys, self->values);
}
void fn_7100eaeba0(UnkKeyed118Array_b45a3* self, UnkKeyed118Iter_b45a3* out) {
    *out = UnkKeyed118Iter_b45a3(self->keys + self->count, self->values + self->count);
}
