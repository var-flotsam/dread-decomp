// Unit 70: removing an entry from a pair of parallel arrays (24-byte values and their string ids): the
// entry's value is destroyed and its id released, the last entry takes its place. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

void fn_71000a179c(UnkReflValue* self, const UnkReflValue* other);  // copy constructor

// Removes the entry in `slot`: its value destroyed and its id released, the last entry moved into its
// place. Returns the slot.
UnkReflValue* fn_7100093a50(UnkPropertyArrays* self, UnkReflValue* slot) {
    UnkReflValue* values = static_cast<UnkReflValue*>(self->values);
    u32 i = slot - values;
    fn_71000a1c60(&values[i]);
    self->ids[i].Reset();
    u32 last = self->count - 1;
    if (last != i) {
        values = static_cast<UnkReflValue*>(self->values);
        fn_71000a179c(&values[i], &values[last]);
        fn_71000a1c60(&static_cast<UnkReflValue*>(self->values)[self->count - 1]);
        CStrId* ids = self->ids;
        last = self->count - 1;
        __builtin_memcpy(&ids[i], &ids[last], sizeof(CStrId));
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&ids[last], &empty, sizeof(empty));
        self->ids[self->count - 1].Reset();
    }
    --self->count;
    return &static_cast<UnkReflValue*>(self->values)[i];
}
