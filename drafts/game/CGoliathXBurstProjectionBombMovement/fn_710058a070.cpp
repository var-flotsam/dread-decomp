// CGoliathXBurstProjectionBombMovement's unit: builds a pair of string ids from a short list of
// ids (at most two are taken). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

#include <stddef.h>

using base::global::CStrId;
using base::global::UnkStrIdBase;

struct UnkStrIdPair_l58a070 {
    CStrId ids[2];
};

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_710058a070(void* self, const UnkStrIdBase* ids, size_t count) {
    UnkStrIdPair_l58a070* pair = static_cast<UnkStrIdPair_l58a070*>(self);
    for (u32 i = 0; i < 2; ++i)
        InitEmptyStrId(&pair->ids[i]);
    u64 n = static_cast<u32>(count);
    if (n < 3) {
        u32 i = 0;
        for (const UnkStrIdBase* it = ids; it != ids + count; ++it, ++i) {
            CStrId id(static_cast<const CStrId&>(*it));
            pair->ids[i] = id;
        }
    }
}
