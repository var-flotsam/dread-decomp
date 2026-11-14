// CSpawnPointComponent unit (unit 2704): base::UnkVector::Insert for the 0x40-byte polymorphic entries
// (moved with fn_710111fd80, destroyed through their virtual destructor): move one in at an index.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkEntry_l1320dc {
    virtual ~UnkEntry_l1320dc();
    u8 pad_0x8[0x38];

    // The entry move, fn_710111fd80.
    UnkEntry_l1320dc(UnkEntry_l1320dc&& other) asm("fn_710111fd80");
};

__attribute__((flatten)) void fn_71011320dc(base::UnkVector<UnkEntry_l1320dc>* self, UnkEntry_l1320dc* value, u32 index) {
    self->Insert(static_cast<UnkEntry_l1320dc&&>(*value), index);
}
