// In unit 437 (CAimComponent's neighbourhood): finds the item with the given id that is a CGun,
// first in the primary grouped list (the last such match wins), then group by group in the
// secondary one. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkAimItem_710034df0c {
    virtual const base::reflection::CType* GetType();

    u8 unk_0x8[0x18];
    u64 id;  // 0x20
};

using UnkAimGroup_710034df0c = base::global::CRntVector<UnkAimItem_710034df0c*>;
using UnkAimGroups_710034df0c = base::global::CRntVector<UnkAimGroup_710034df0c>;

struct UnkAimOwner_710034df0c {
    u8 unk_0x0[0xc8];
    UnkAimGroups_710034df0c primary;    // 0xc8
    UnkAimGroups_710034df0c secondary;  // 0xf0
};

template <const base::reflection::CClass* (*Getter)()>
static inline UnkAimItem_710034df0c* CastTo(UnkAimItem_710034df0c* item) {
    return item && fn_710009d50c(item->GetType(), Getter()) ? item : nullptr;
}

// The cast of the last item in `group` whose id is `id`.
static inline UnkAimItem_710034df0c* FindInGroup(UnkAimGroup_710034df0c* group, const u64* id,
                                                  UnkAimItem_710034df0c* found) {
    for (UnkAimItem_710034df0c** it = group->data; it != group->data + group->count; ++it) {
        if ((*it)->id == *id)
            found = CastTo<fn_71008351ac>(*it);
    }
    return found;
}

UnkAimItem_710034df0c* fn_710034df0c(UnkAimOwner_710034df0c* self, const u64* id) {
    UnkAimItem_710034df0c* found = nullptr;
    for (UnkAimGroup_710034df0c* g = self->primary.data; g != self->primary.data + self->primary.count; ++g)
        found = FindInGroup(g, id, found);
    if (found)
        return found;

    for (UnkAimGroup_710034df0c* g = self->secondary.data; g != self->secondary.data + self->secondary.count;
         ++g) {
        UnkAimItem_710034df0c* match = FindInGroup(g, id, nullptr);
        if (match)
            return match;
    }
    return found;
}
