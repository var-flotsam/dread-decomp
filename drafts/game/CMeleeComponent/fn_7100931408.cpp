// CMeleeComponent's unit (hint only): the gun with the given id among the grouped items, the first
// list (+0xc8) searched whole before the second (+0xf0), group by group; within a group the last
// item with the id counts. Null when it is not a CGun or there is none. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkItem_l931408 : UnkReflObject {
    u8 pad_0x8[0x18];
    u64 id;  // 0x20
};

using UnkGroups_l931408 = base::global::CRntVector<base::global::CRntVector<UnkItem_l931408*>>;

struct UnkItemOwner_l931408 {
    u8 pad_0x0[0xc8];
    UnkGroups_l931408 primary;    // 0xc8
    UnkGroups_l931408 secondary;  // 0xf0
};

static inline UnkItem_l931408* AsGun(UnkItem_l931408* item) {
    if (!item)
        return nullptr;
    return fn_710009d50c(item->vf_0x0(), fn_71008351ac()) ? item : nullptr;
}

UnkItem_l931408* fn_7100931408(UnkItemOwner_l931408* self, const u64* id) {
    UnkItem_l931408* gun = nullptr;
    for (auto* g = self->primary.data; g != self->primary.data + self->primary.count; ++g) {
        for (UnkItem_l931408** it = g->data; it != g->data + g->count; ++it) {
            if ((*it)->id == *id)
                gun = AsGun(*it);
        }
    }
    for (auto* g = self->secondary.data; !gun && g != self->secondary.data + self->secondary.count; ++g) {
        for (UnkItem_l931408** it = g->data; it != g->data + g->count; ++it) {
            if ((*it)->id == *id)
                gun = AsGun(*it);
        }
    }
    return gun;
}
