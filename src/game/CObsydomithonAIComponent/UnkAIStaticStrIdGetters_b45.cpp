// A family of four getters (one each in the CObsydomithonAIComponent, CQuetzoaXAIComponent,
// CQuetzoaAIComponent and CSluggerAIComponent units, from tools/tu_map.py; the first is the
// original): returns a copy of a static string id (base::global::CStrId's data pointer) of its
// unit, taking a reference unless it is the pool's empty entry. Names are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

// Returned in x8 (a type with a user-written destructor); the destructor is not used here.
struct UnkStrIdCopy_b45a2 {
    base::global::UnkStringData* data;
    ~UnkStrIdCopy_b45a2();
};

// The comparison is written on integers: as pointers Clang swaps the cmp operands.
#define UNK_STATIC_STRID_GETTER(fn, var)                                                \
    __attribute__((visibility("hidden"))) extern base::global::UnkStringData* var;      \
    UnkStrIdCopy_b45a2 fn() {                                                              \
        base::global::UnkStringData* p = var;                                           \
        UnkStrIdCopy_b45a2 r{p};                                                           \
        if (reinterpret_cast<u64>(p) != reinterpret_cast<u64>(g_unk_7101d47108) + 0x10)    \
            ++p->ref_count;                                                                \
        return r;                                                                          \
    }

UNK_STATIC_STRID_GETTER(fn_710062ea44, g_unk_71726f9460)
UNK_STATIC_STRID_GETTER(fn_7100640808, g_unk_71726fab48)
UNK_STATIC_STRID_GETTER(fn_710064b3c0, g_unk_71726fc040)
UNK_STATIC_STRID_GETTER(fn_71006c6b34, g_unk_7172708020)
