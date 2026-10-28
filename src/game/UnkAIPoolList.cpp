// From unit 2702 of tools/tu_map.py (0x71010f41a0-0x71011173fc, CAIComponent by reflection; the unit
// boundary is of medium confidence): the constructor of a container of 0x28-byte nodes over an
// embedded block pool (base/UnkParentPoolList.h). Names are placeholders.
// Holds fn_71010f8804.
#include "base/UnkParentPoolList.h"
#include "types.h"

extern char unk_7101cb25d8[];  // the base part's vtable (reached through the GOT)
extern char unk_7101cb25b8[];  // the container's vtable (reached through the GOT)

void fn_71010f8804(UnkParentPoolList* self, u32 n, u32 /*unused*/, u32 flags, base::UnkAllocator* a) {
    refl_ParentPoolListCtor<0x28, unk_7101cb25d8, unk_7101cb25b8>(self, n, flags, a);
}
