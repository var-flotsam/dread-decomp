// CEntity's unit (tools/tu_map.py unit 2523): the copy assignment of a base::global::CRntVector of 0x40-byte
// entries, a copy of fn_7101131d60 (src/game/CSpawnPointComponent/CSpawnPointComponent_b42.cpp) with
// this unit's entry move (fn_71007b1928) and append (fn_7100d8865c), as the copy constructor beside it
// (fn_7100d88548, src/game/CEntity/CEntity_VectorCopy_b43.cpp). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CDemolitionBlockComponent.h"
#include "game/UnkEntry_b43a3.h"
#include "game/UnkTU2523.h"
#include "types.h"

base::global::CRntVector<UnkEntry_b43a3>* fn_7100d88824(base::global::CRntVector<UnkEntry_b43a3>* self,
                                               const base::global::CRntVector<UnkEntry_b43a3>* other) {
    if (self == other)
        return self;
    for (u64 i = 0; i < self->count; ++i)
        self->data[i].~UnkEntry_b43a3();
    self->count = 0;
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
    return self;
}
