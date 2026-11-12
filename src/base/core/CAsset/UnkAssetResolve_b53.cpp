// base::core::CAsset's unit (unit 24 of tools/tu_map.py): the resource a string id and a file-path id
// name, when both are set (fn_710002038c in the resource registry); an empty reference otherwise.
// This unit holds fn_710001737c, inlined here: a resource whose last reference goes is handed back to
// the registry (fn_7100020468). Names are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkStrIdPathPair.h"
#include "game/UnkTU27.h"
#include "types.h"

// The resource registry (src/game_s7/fn_7100d92158.cpp registers its descriptors; reached through
// the GOT).
struct UnkRegistry_7100d92158;
extern UnkRegistry_7100d92158* g_unk_7101d4bbc0;

void fn_7100020468(UnkRegistry_7100d92158* registry, UnkCountedResource* resource);

// A counted reference to a resource, as the registry returns it (in memory); as in
// src/base/core/CAssetLink.cpp, with fn_710001737c inlined (same behaviour).
struct UnkAssetResRef {
    UnkCountedResource* ptr;

    UnkAssetResRef() : ptr(nullptr) {}
    UnkAssetResRef& operator=(const UnkAssetResRef& other) {
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_7100020468(g_unk_7101d4bbc0, old);
            ptr = p;
        }
        return *this;
    }
    ~UnkAssetResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_7100020468(g_unk_7101d4bbc0, ptr);
    }
};

UnkAssetResRef fn_7100017878(void* self, const UnkStrIdPathPair* ids) {
    UnkAssetResRef res;
    if (ids->id.data != &g_unk_7101d47108->empty && ids->path.data != &g_unk_7101d47200->empty)
        res = fn_710002038c(g_unk_7101d4bbc0, ids, true, nullptr);
    return res;
}
