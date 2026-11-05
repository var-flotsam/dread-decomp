// CCentralUnitAIComponent unit (tools/tu_map.py, the name from reflection): maps an id to a kind: 0x11..0x15
// for the five ids at 0x71726c4110, 0x118, 0x120, 0x130 and 0x140, else -1. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_71726c4110 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726c4118 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726c4120 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726c4130 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726c4140 __attribute__((visibility("hidden")));

struct UnkCentralUnitAI_b67a2;

s64 fn_71004057d8(UnkCentralUnitAI_b67a2* self, const CStrId* id) {
    if (id->data == g_unk_71726c4110.data)
        return 0x11;
    if (id->data == g_unk_71726c4118.data)
        return 0x12;
    if (id->data == g_unk_71726c4120.data)
        return 0x13;
    if (id->data == g_unk_71726c4130.data)
        return 0x14;
    if (id->data == g_unk_71726c4140.data)
        return 0x15;
    return -1;
}
