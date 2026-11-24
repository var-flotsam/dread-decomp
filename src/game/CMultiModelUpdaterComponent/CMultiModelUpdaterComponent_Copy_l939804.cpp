// CMultiModelUpdaterComponent's unit (tools/tu_map.py unit 1426): the copy constructor of a model
// map value: a name, a file path, a counted resource and one more word. Both ids start empty, then
// every member is assigned from the source. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkModelValue_b0119.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CStrId;

// A counted reference to the loaded resource (as UnkResRef, base/UnkResKey.h, with the source's
// pointer read first).
struct UnkModelResRef_l939804 {
    UnkCountedResource* ptr;

    UnkModelResRef_l939804() : ptr(nullptr) {}
    UnkModelResRef_l939804& operator=(const UnkModelResRef_l939804& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
};

struct UnkModel_l939804 {
    CStrId name;
    CFilePathStrId path;
    UnkModelResRef_l939804 resource;
    u64 field_0x18;
};
static_assert(sizeof(UnkModel_l939804) == sizeof(UnkModelValue_b0119), "model value size");

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

void CMultiModelUpdaterComponent_CopyConstructModelMapValue(UnkModelValue_b0119* self, const UnkModelValue_b0119* other) {
    UnkModel_l939804* model = reinterpret_cast<UnkModel_l939804*>(self);
    const UnkModel_l939804* from = reinterpret_cast<const UnkModel_l939804*>(other);
    InitEmptyStrId(&model->name);
    InitEmptyPath(&model->path);
    new (&model->resource) UnkModelResRef_l939804();
    model->name = from->name;
    model->path = from->path;
    model->resource = from->resource;
    model->field_0x18 = from->field_0x18;
}
