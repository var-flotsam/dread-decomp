// From unit 32 of tools/tu_map.py (0x71000255c4-0x7100027774), base::core::CAssetLink's unit (its
// static initialiser is src/game_s1/fn_71000275b0.cpp): a string id, a file-path id and the
// resource the pair resolves to (base/UnkStrIdPathPair.h's UnkStrIdPathResource). Assigning new
// ids resolves the resource again through the resource registry. Names are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkStrIdPathPair.h"
#include "types.h"

// The resource registry (src/game_s7/fn_7100d92158.cpp registers its descriptors; reached through
// the GOT).
struct UnkRegistry_7100d92158;
extern UnkRegistry_7100d92158* g_unk_7101d4bbc0;

// A counted reference to a resource, as the registry returns it (in memory).
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
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkAssetResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

// The resource the ids name, looked up in the registry (the flag's and the last argument's
// meanings are unknown; the last is a 64-bit word, null here).
UnkAssetResRef fn_710002038c(UnkRegistry_7100d92158* registry, const UnkStrIdPathPair* ids, bool a,
                             void* b);

void fn_71000267f8(UnkStrIdPathResource* self, const UnkStrIdPathPair& other) {
    self->ids.id = other.id;
    self->ids.path = other.path;
    UnkAssetResRef res =
        self->ids.id.data != &g_unk_7101d47108->empty && self->ids.path.data != &g_unk_7101d47200->empty
            ? fn_710002038c(g_unk_7101d4bbc0, &self->ids, true, nullptr)
            : UnkAssetResRef();
    reinterpret_cast<UnkAssetResRef&>(self->resource) = res;
}
