// CMultiModelUpdaterComponent's unit (tools/tu_map.py unit 1426): builds a model map value from
// a name, a file path and a counted resource; its last word starts at zero. Names and layouts are
// placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CStrId;

// As its caller declares it (CMultiModelUpdaterComponent_Models_ovb0119.cpp).
struct UnkModelValue_b0119 {
    u8 data[0x20];
};

// A counted reference to the loaded resource (as UnkResRef, base/UnkResKey.h).
struct UnkModelResRef_l938ba0 {
    UnkCountedResource* ptr;

    UnkModelResRef_l938ba0() : ptr(nullptr) {}
    void Set(UnkCountedResource* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
};

struct UnkModel_l938ba0 {
    CStrId name;
    CFilePathStrId path;
    UnkModelResRef_l938ba0 resource;
    u64 field_0x18;
};
static_assert(sizeof(UnkModel_l938ba0) == sizeof(UnkModelValue_b0119), "model value size");

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

static inline void InitEmptyPath(CFilePathStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    id->data = &g_unk_7101d47200->empty;
}

void fn_7100938ba0(UnkModelValue_b0119* self, const CStrId* name, const CFilePathStrId* path,
                   UnkCountedResource* resource) {
    UnkModel_l938ba0* model = reinterpret_cast<UnkModel_l938ba0*>(self);
    InitEmptyStrId(&model->name);
    InitEmptyPath(&model->path);
    new (&model->resource) UnkModelResRef_l938ba0();
    model->name = *name;
    model->path = CFilePathStrId(*path);
    model->resource.Set(resource);
    model->field_0x18 = 0;
}
