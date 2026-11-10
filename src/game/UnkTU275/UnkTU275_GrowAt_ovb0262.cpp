// Unit 275: the element accessor of a growable array of 16-byte elements that are copied word by
// word and start out zeroed (base/UnkVector.h, GrowAt). Names are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32_ovb0262r;

struct UnkWords_ovb0262r {
    u32 words[4];
    UnkWords_ovb0262r() {
        words[0] = 0;
        words[1] = 0;
        words[2] = 0;
        words[3] = 0;
    }
    // Copied through an aliasing type: the original reloads the array's pointer and count afterwards.
    UnkWords_ovb0262r(const UnkWords_ovb0262r& o) {
        for (int i = 0; i < 4; ++i)
            reinterpret_cast<a_u32_ovb0262r*>(words)[i] = reinterpret_cast<const a_u32_ovb0262r*>(o.words)[i];
    }
};

UnkWords_ovb0262r* fn_71001f7f78(base::UnkVector<UnkWords_ovb0262r>* self, u32 index) { return &self->GrowAt(index); }
