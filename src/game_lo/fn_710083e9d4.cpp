// Finds an item with the given id in one of two grouped item lists (chosen by `primary`) that
// also accepts the query through a virtual check; null if there is none.
#include "base/UnkVector.h"
#include "types.h"

struct UnkItem_710083e9d4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool vf_0x30();

    u8 unk_0x8[0x18];
    u64 id;  // 0x20
};

using UnkGroups_710083e9d4 = base::UnkVector<base::UnkVector<UnkItem_710083e9d4*>>;

static inline UnkItem_710083e9d4* Find(UnkGroups_710083e9d4& groups, const u64* id) {
    for (base::UnkVector<UnkItem_710083e9d4*>* g = groups.data; g != groups.data + groups.count; ++g) {
        for (UnkItem_710083e9d4** it = g->data; it != g->data + g->count; ++it) {
            if ((*it)->id == *id && (*it)->vf_0x30())
                return *it;
        }
    }
    return nullptr;
}

struct UnkStruct_710083e9d4 {
    u8 unk_0x0[0xc8];
    UnkGroups_710083e9d4 primary;    // 0xc8
    UnkGroups_710083e9d4 secondary;  // 0xf0
};

UnkItem_710083e9d4* fn_710083e9d4(UnkStruct_710083e9d4* self, const u64* id, bool primary) {
    if (primary)
        return Find(self->primary, id);
    return Find(self->secondary, id);
}
