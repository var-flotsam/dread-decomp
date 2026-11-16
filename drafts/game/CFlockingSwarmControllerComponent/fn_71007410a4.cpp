// CFlockingSwarmControllerComponent unit: the swarm's group at an index, growing the array of groups
// (0x38 bytes: a list of ids, a word and a name) to reach it and value-initialising the new ones.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkRedenkiNamedList_ovb0084;
void fn_710065b430(base::global::CRntVector<UnkRedenkiNamedList_ovb0084>* self, u32 n);

// An id starting as the pool's empty string.
struct UnkGroupName_l7410a4 {
    UnkStringData* data;

    UnkGroupName_l7410a4() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkSwarmGroup_l7410a4 {
    base::global::CRntVector<u32> members{0xf};
    u32 field_0x28 = 3;
    UnkGroupName_l7410a4 name;  // +0x30
};
static_assert(sizeof(UnkSwarmGroup_l7410a4) == 0x38, "UnkSwarmGroup_l7410a4 placeholder layout");

using UnkSwarmGroups_l7410a4 = base::global::CRntVector<UnkSwarmGroup_l7410a4>;

static inline void Reserve(UnkSwarmGroups_l7410a4* groups, u32 n) {
    fn_710065b430(reinterpret_cast<base::global::CRntVector<UnkRedenkiNamedList_ovb0084>*>(groups), n);
}

UnkGroup_fs* fn_71007410a4(UnkArray_fs* array, u32 index) {
    auto* groups = reinterpret_cast<UnkSwarmGroups_l7410a4*>(array);
    if (groups->field_0x14 && groups->count <= index) {
        u32 cap = groups->capacity;
        if (cap <= index && groups->step != 0)
            cap = (index / groups->step + 1) * groups->step;
        Reserve(groups, cap);
        u32 n = index + 1;
        Reserve(groups, n);
        for (u32 i = groups->count; i < n; ++i)
            new (&groups->data[i]) UnkSwarmGroup_l7410a4();
        groups->count = n;
    }
    return reinterpret_cast<UnkGroup_fs*>(&groups->data[index]);
}
