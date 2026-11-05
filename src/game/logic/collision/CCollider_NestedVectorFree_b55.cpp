// Unit 2503 (game::logic::collision::CCollider, tools/tu_map.py): a copy of fn_71001254a4
// (src/base/UnkUnit109/UnkNestedVectorFree_b54.cpp), the free of a base::UnkVector of 0x28-byte
// records, each a vector of its own. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkVectorRecord28_b55 {
    base::UnkVector<u8> items;
};

void fn_7100d43b98(base::UnkVector<UnkVectorRecord28_b55>* self) {
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].items.Free();
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}
