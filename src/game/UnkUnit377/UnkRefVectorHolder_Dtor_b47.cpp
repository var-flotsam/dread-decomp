// Unit 377 (tools/tu_map.py): the destructor of a holder of a growable array (+0x10, base/UnkVector.h)
// and an intrusively counted object (+0x0): frees the array, then releases the object. Names are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/UnkTU377.h"

struct UnkRefVectorHolder_b47 {
    base::UnkRefPtrMember ref;    // 0x0
    u8 pad_0x8[0x8];
    base::UnkVector<u8> items;  // 0x10

    ~UnkRefVectorHolder_b47() { items.Free(); }
};

void fn_71002b67e8(UnkRefVectorHolder_b47* self) { self->~UnkRefVectorHolder_b47(); }
