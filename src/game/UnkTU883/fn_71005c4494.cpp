// CHydrogigaAIComponent's unit: fn_71005c4494 adds a key and a copy of its 0x58-byte entry to the end of
// the keyed table (growing it by its step when full) and returns the entry added.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkPooledStrId.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkHydrogigaEntry_b0166.h"
#include "types.h"

using base::global::CStrId;

static_assert(sizeof(UnkHydrogigaEntry_b0166) == 0x58, "layout");

using UnkHydrogigaTable_b0166 = base::global::CRntSmallDictionary<base::UnkStrIdMember, UnkHydrogigaEntry_b0166>;

void ReserveHydrogigaKeyedTable(UnkHydrogigaTable_b0166* self, u32 n);

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkHydrogigaEntry_b0166* fn_71005c4494(UnkHydrogigaTable_b0166* self, const base::UnkStrIdMember& key,
                                       const UnkHydrogigaEntry_b0166& value) {
    if (self->count == self->capacity)
        ReserveHydrogigaKeyedTable(self, self->count + self->step);
    using KeyId = base::UnkPooledStrId<g_unk_7101d47108>;
    new (&self->keys[self->count]) KeyId(reinterpret_cast<const KeyId&>(key));
    new (&self->values[self->count]) UnkHydrogigaEntry_b0166(value);
    ++self->count;
    UnkHydrogigaEntry_b0166* end = &self->values[self->count];
    return self->count ? end - 1 : end;
}
