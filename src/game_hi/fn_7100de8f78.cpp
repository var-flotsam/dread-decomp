// Returns a new reference to a ref-counted handle's object and records the object once in a
// function-local static registry (a fixed array of handles behind a u32 count).
#include "base/UnkStrIdRegistry.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

// The empty id is the string pool's empty entry (0x10 past the pool) and is never counted.
static inline bool IsCounted(UnkStringData* p) {
    return p != &g_unk_7101d47108->empty;
}

struct UnkRefRegistry {
    u32 count;
    u8 data[256 * sizeof(CStrId)];  // capacity unknown
    UnkRefRegistry() : count(0) {}
    ~UnkRefRegistry();  // fn_7100de4928
    CStrId* begin() { return reinterpret_cast<CStrId*>(data); }
    CStrId* end() { return begin() + count; }
    void Add(const CStrId& r) {
        UnkStringData* p = r.data;
        if (IsCounted(p))
            ++p->ref_count;
        begin()[count].data = p;
        ++count;
    }
};

static UnkRefRegistry& GetRegistry_de8f78() {
    static UnkRefRegistry s;
    return s;
}

CStrId fn_7100de8f78(const CStrId& src) {
    CStrId r(src);
    for (CStrId *it = GetRegistry_de8f78().begin(), *e = GetRegistry_de8f78().end(); it != e; ++it)
        if (it->data == r.data)
            return r;
    GetRegistry_de8f78().Add(r);
    return r;
}

// Not an original function: a stand-in for the other statics that the original TU's merged
// globals place between the registry's guard and the registry (0x6a8 bytes apart). Without it
// Clang's global merging puts them 8 bytes apart and the guarded init addresses differ.
static u64 s_layoutPadA_de8f78;
static u64 s_layoutPadB_de8f78[0x6a0 / 8];
u64 LayoutPad_de8f78(u32 i) { return s_layoutPadA_de8f78 += s_layoutPadB_de8f78[i]++; }
