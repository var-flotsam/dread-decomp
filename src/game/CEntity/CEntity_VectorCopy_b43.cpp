// CEntity's unit (tools/tu_map.py): one more copy of fn_7101131a84
// (src/game/CSpawnPointComponent/CSpawnPointComponent_b42.cpp), the copy constructor of a base::UnkVector of
// 0x40-byte entries, with this unit's own entry move (fn_71007b1928) and append (fn_7100d8865c).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CDemolitionBlockComponent.h"
#include "types.h"

// As src/clones/clones_04.cpp declares it: moves the entry at the second pointer into the first.

struct UnkEntry_b43a3 {
    virtual ~UnkEntry_b43a3();
    u8 pad_0x8[0x38];
};

void fn_7100d8865c(base::UnkVector<UnkEntry_b43a3>* self, const UnkEntry_b43a3* value);

void fn_7100d88548(base::UnkVector<UnkEntry_b43a3>* self, const base::UnkVector<UnkEntry_b43a3>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    u32 n = other->capacity;
    if (self->capacity < n) {
        UnkEntry_b43a3* p = static_cast<UnkEntry_b43a3*>(self->allocator->vf_0x40((u64)n * sizeof(UnkEntry_b43a3), 8, self->flags));
        if (UnkEntry_b43a3* src = self->data) {
            UnkEntry_b43a3* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                fn_71007b1928(dst, src);
                (src++)->~UnkEntry_b43a3();
                ++dst;
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (const UnkEntry_b43a3 *it = other->data, *end = other->data + other->count; it != end; ++it)
        fn_7100d8865c(self, it);
}
