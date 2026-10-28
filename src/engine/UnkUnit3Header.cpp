// tools/tu_map.py unit 3 (0x7100001860), a unit with nothing but its static initialiser: the header
// every unit's initialiser starts with (6, then the "sources" and "binaries" ids, this unit in that
// order).
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkUnit3Header {
    u32 field_0x0;
    CStrId sources;
    CStrId binaries;
};
extern UnkUnit3Header g_unk_7101d474b8 __attribute__((visibility("hidden")));

void fn_7100001860() {
    g_unk_7101d474b8.field_0x0 = 6;
    new (&g_unk_7101d474b8.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d474b8.sources, g_unk_7101a19000);
    new (&g_unk_7101d474b8.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d474b8.binaries, g_unk_7101a19000);
}
