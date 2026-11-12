// Returns a copy of a static string id of the CAIComponent unit (tools/tu_map.py), taking a
// reference unless it is the pool's empty entry. Same shape as fn_71006c6b34 in
// src/game/CObsydomithonAIComponent/UnkAIStaticStrIdGetters_b45.cpp. Names are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

// Returned in x8 (a type with a user-written destructor); the destructor is not used here.
struct UnkStrIdCopy_b46a3 {
    base::global::UnkStringData* data;
    ~UnkStrIdCopy_b46a3();
};

__attribute__((visibility("hidden"))) extern base::global::UnkStringData* g_unk_7172a06328;

UnkStrIdCopy_b46a3 fn_710111203c(void*) {
    base::global::UnkStringData* p = g_unk_7172a06328;
    UnkStrIdCopy_b46a3 r{p};
    if (reinterpret_cast<u64>(p) != reinterpret_cast<u64>(g_unk_7101d47108) + 0x10)
        ++p->ref_count;
    return r;
}
