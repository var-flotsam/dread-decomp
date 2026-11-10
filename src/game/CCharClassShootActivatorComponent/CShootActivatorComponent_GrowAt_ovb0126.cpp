// CCharClassShootActivatorComponent's unit (tools/tu_map.py): the element of the array of 0x48-byte entries
// (a callback set and a string id, as src/game/CCharClassShootActivatorComponent/fn_71008947b8.cpp lays them
// out) at an index (fn_7100892528). When the array may grow and the index is past the end, it reserves the
// next multiple of the step and then exactly index + 1 entries (the array's Reserve, fn_7100895498) and
// value-initialises the new entries: an empty array of callables (step 10, flags 0xf, the default
// allocator) under the set's two vtables, no owner and the empty string id. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

extern void* g_unk_7101a20998[];  // the callback set's base vtable
extern void* g_unk_7101a33fa0[];  // the callback set's vtable

struct UnkEntryVector;
void fn_7100895498(UnkEntryVector* v, u32 n);

struct UnkGrowSetBase_ovb0126 {
    char* vtable;
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkGrowSetBase_ovb0126() : vtable(reinterpret_cast<char*>(g_unk_7101a20998) + 0x10), field_0x30(0) {}
};

struct UnkGrowSet_ovb0126 : UnkGrowSetBase_ovb0126 {
    base::UnkRefHandle owner;

    UnkGrowSet_ovb0126() {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        owner.ptr = nullptr;
    }
};

struct UnkGrowId_ovb0126 {
    UnkStringData* data;

    UnkGrowId_ovb0126() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkGrowEntry_ovb0126 {
    UnkGrowSet_ovb0126 set;
    UnkGrowId_ovb0126 id;
};
static_assert(sizeof(UnkGrowEntry_ovb0126) == 0x48, "entry size");

struct UnkGrowVector_ovb0126 {
    UnkGrowEntry_ovb0126* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
};

UnkGrowEntry_ovb0126* fn_7100892528(void* self, u32 key) {
    auto* v = static_cast<UnkGrowVector_ovb0126*>(self);
    if (v->field_0x14 && v->count <= key) {
        u32 cap = v->capacity;
        if (cap <= key && v->step != 0)
            cap = (key / v->step + 1) * v->step;
        fn_7100895498(reinterpret_cast<UnkEntryVector*>(v), cap);
        u32 n = key + 1;
        fn_7100895498(reinterpret_cast<UnkEntryVector*>(v), n);
        for (u32 i = v->count; i < n; ++i)
            new (&v->data[i]) UnkGrowEntry_ovb0126();
        v->count = n;
    }
    return &v->data[key];
}
