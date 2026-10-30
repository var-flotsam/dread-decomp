// Returns a new reference to a ref-counted handle's object and records the object once in a
// function-local static registry (a fixed array of handles behind a u32 count).
#include <new>

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
    ~UnkRefRegistry() asm("fn_7100de4928");  // fn_7100de4928
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

#if defined(__aarch64__)
static UnkRefRegistry& GetRegistry_de8f78() {
    static UnkRefRegistry s;
    return s;
}
#else
// On the port the registry is the image's (0x71727bf4d8, its guard 0x71727bee30), which the lifted
// fn_7100de917c copies into the game manager's id table: a function-local static here would be the
// executable's own copy, and the manager would find no ids (fn_7100de9540 then indexed an empty table).
extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
extern UnkRefRegistry g_unk_71727bf4d8 __attribute__((visibility("hidden")));
extern u64 g_unk_71727bee30 __attribute__((visibility("hidden")));
static UnkRefRegistry& GetRegistry_de8f78() {
    if (!(*reinterpret_cast<volatile u8*>(&g_unk_71727bee30) & 1) && __cxa_guard_acquire(&g_unk_71727bee30)) {
        new (&g_unk_71727bf4d8) UnkRefRegistry;
        __cxa_guard_release(&g_unk_71727bee30);
    }
    return g_unk_71727bf4d8;
}
#endif

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
