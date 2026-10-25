// Returns a new reference to a ref-counted handle's object and records the object once in a
// function-local static registry (a fixed array of handles behind a u32 count).
#include "base/global/CStrId.h"
#include "types.h"

// A string id's data, seen with a plain count.
struct UnkRefObj {
    char pad[8];
    s32 refCount;
};

// The empty handle is the string pool's empty entry (0x10 past the pool) and is never counted.
static inline bool IsCounted(UnkRefObj* p) {
    return (u64)p != (u64)g_unk_7101d47108 + 0x10;
}

struct UnkRef {
    UnkRefObj* ptr;
    explicit UnkRef(UnkRefObj* p) : ptr(p) {
        if (IsCounted(ptr))
            ++ptr->refCount;
    }
    UnkRef(const UnkRef& o) : UnkRef(o.ptr) {}
};

struct UnkRefRegistry {
    u32 count;
    u8 data[256 * sizeof(UnkRef)];  // capacity unknown
    UnkRefRegistry() : count(0) {}
    ~UnkRefRegistry();  // fn_7100de4928
    UnkRef* begin() { return reinterpret_cast<UnkRef*>(data); }
    UnkRef* end() { return begin() + count; }
    void Add(const UnkRef& r) {
        UnkRefObj* p = r.ptr;
        if (IsCounted(p))
            ++p->refCount;
        begin()[count].ptr = p;
        ++count;
    }
};

static UnkRefRegistry& GetRegistry_de8f78() {
    static UnkRefRegistry s;
    return s;
}

UnkRef fn_7100de8f78(const UnkRef& src) {
    UnkRef r(src.ptr);
    for (UnkRef *it = GetRegistry_de8f78().begin(), *e = GetRegistry_de8f78().end(); it != e; ++it)
        if (it->ptr == r.ptr)
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
