// The in-place constructor (fn_0x28 of the reflected class in src/reflection/class_descriptors_00.cpp, tag
// 71003ecb04) of the 0x30-byte record whose move assignment is src/game_s2/fn_71003eeca8.cpp, value-initialised (the
// whole record is cleared first): an array (UnkVector, flags 0xf) and a default string id at +0x28 (the pool is
// created first if there is none). Defined under the `void()` symbol the descriptor declares. Names and layouts
// are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkVecStrIdRecord_A2_b76 {
    base::UnkVector<void*> field_0x0;
    UnkStringData* field_0x28;
};

void fn_71003eea0c_A2_b76(UnkVecStrIdRecord_A2_b76* self) asm("_Z13fn_71003eea0cv");
void fn_71003eea0c_A2_b76(UnkVecStrIdRecord_A2_b76* self) {
    __builtin_memset(self, 0, sizeof(*self));
    new (&self->field_0x0) base::UnkVector<void*>(0xf);
    self->field_0x28 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x28 = &g_unk_7101d47108->empty;
}
