// CAreaFXComponent unit (tools/tu_map.py unit 1263): fn_7100802418 resets two string ids to the empty
// id and, when the definition at +0x3f8 has both or neither of its ids (+0x90, +0x98) set, copies them
// out; whether it did. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkAreaFXIds_ovb0081 {
    u8 pad_0x0[0x90];
    CStrId first;   // 0x90
    CStrId second;  // 0x98
};

struct UnkAreaFXIdHolder_ovb0081 {
    u8 pad_0x0[0x3f8];
    UnkAreaFXIds_ovb0081* ids;  // 0x3f8
};

bool fn_7100802418(UnkAreaFXIdHolder_ovb0081* self, CStrId* first, CStrId* second) {
    *first = g_unk_7101d470e0;
    *second = g_unk_7101d470e0;
    UnkAreaFXIds_ovb0081* ids = self->ids;
    if (!ids)
        return false;
    UnkStringPool* pool = g_unk_7101d47108;
    if ((ids->first.data == &pool->empty) ^ (ids->second.data == &pool->empty))
        return false;
    *first = ids->first;
    *second = ids->second;
    return true;
}
