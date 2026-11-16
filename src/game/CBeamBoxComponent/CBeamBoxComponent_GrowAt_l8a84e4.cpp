// CBeamBoxComponent's unit (tools/tu_map.py unit 1355): the element of the array of 0x48-byte entries (a
// callback set and a string id) at an index, growing the array (fn_71008ab4e0) and value-initialising the new
// entries when growing is allowed and the index is past the end. Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CBeamBoxComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkBeamSetBase_l8a84e4 {
    char* vtable;
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkBeamSetBase_l8a84e4() : vtable(reinterpret_cast<char*>(g_unk_7101a20998) + 0x10), field_0x30(0) {}
};

struct UnkBeamSet_l8a84e4 : UnkBeamSetBase_l8a84e4 {
    base::UnkRefHandle owner;

    UnkBeamSet_l8a84e4() {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        owner.ptr = nullptr;
    }
};

struct UnkBeamId_l8a84e4 {
    UnkStringData* data;

    UnkBeamId_l8a84e4() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkBeamEntry_l8a84e4 {
    UnkBeamSet_l8a84e4 set;
    UnkBeamId_l8a84e4 id;
};
static_assert(sizeof(UnkBeamEntry_l8a84e4) == 0x48, "entry size");

struct UnkBeamVector_l8a84e4 {
    UnkBeamEntry_l8a84e4* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
};

UnkEntry* fn_71008a84e4(void* self, u32 key) {
    auto* v = static_cast<UnkBeamVector_l8a84e4*>(self);
    if (v->field_0x14 && v->count <= key) {
        u32 cap = v->capacity;
        if (cap <= key && v->step != 0)
            cap = (key / v->step + 1) * v->step;
        fn_71008ab4e0(reinterpret_cast<UnkEntryVector*>(v), cap);
        u32 n = key + 1;
        fn_71008ab4e0(reinterpret_cast<UnkEntryVector*>(v), n);
        for (u32 i = v->count; i < n; ++i)
            new (&v->data[i]) UnkBeamEntry_l8a84e4();
        v->count = n;
    }
    return reinterpret_cast<UnkEntry*>(&v->data[key]);
}
