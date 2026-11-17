// CCharClassFakeDropEvent's unit (tools/tu_map.py unit 1780): finds the missile gun with the given id, first
// in the primary grouped item lists, then group by group in the secondary ones; null if there is none.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkItem_lbab1e8 {
    virtual const base::reflection::CType* vf_0x0();

    u8 pad_0x8[0x18];
    u64 id;  // 0x20
};

using UnkGroup_lbab1e8 = base::global::CRntVector<UnkItem_lbab1e8*>;
using UnkGroups_lbab1e8 = base::global::CRntVector<UnkGroup_lbab1e8>;

struct UnkGunOwner_lbab1e8 {
    u8 pad_0x0[0xc8];
    UnkGroups_lbab1e8 primary;    // 0xc8
    UnkGroups_lbab1e8 secondary;  // 0xf0
};

// The item if it is a CMissileBaseGun, else null.
static inline UnkItem_lbab1e8* AsMissileGun(UnkItem_lbab1e8* item) {
    if (!item)
        return nullptr;
    return fn_710009d50c(item->vf_0x0(), fn_710084de80()) ? item : nullptr;
}

// The last item of the group with the given id, as a missile gun.
static inline UnkItem_lbab1e8* FindInGroup(UnkGroup_lbab1e8* g, const u64* id) {
    UnkItem_lbab1e8* found = nullptr;
    for (UnkItem_lbab1e8** it = g->data; it != g->data + g->count; ++it) {
        if ((*it)->id == *id)
            found = AsMissileGun(*it);
    }
    return found;
}

// The last item of all the groups with the given id, as a missile gun.
static inline UnkItem_lbab1e8* FindInGroups(UnkGroups_lbab1e8* groups, const u64* id) {
    UnkItem_lbab1e8* found = nullptr;
    for (UnkGroup_lbab1e8* g = groups->data; g != groups->data + groups->count; ++g) {
        for (UnkItem_lbab1e8** it = g->data; it != g->data + g->count; ++it) {
            if ((*it)->id == *id)
                found = AsMissileGun(*it);
        }
    }
    return found;
}

void* fn_7100bab1e8(void* owner, void* key) {
    UnkGunOwner_lbab1e8* self = static_cast<UnkGunOwner_lbab1e8*>(owner);
    const u64* id = static_cast<const u64*>(key);

    UnkItem_lbab1e8* found = FindInGroups(&self->primary, id);
    if (!found) {
        for (UnkGroup_lbab1e8* g = self->secondary.data; g != self->secondary.data + self->secondary.count; ++g) {
            found = FindInGroup(g, id);
            if (found)
                break;
        }
    }
    return found;
}
