// CInventoryComponent unit: maps an item id to its slot index (0 to 24), or -1 for an id it does not know.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "types.h"

using base::global::CStrId;

struct CInventoryComponent;

s64 fn_7101143838(CInventoryComponent* self, const CStrId& id) {
    if (id == g_unk_7172a092b8) return 0;
    if (id == g_unk_7172a092c0) return 1;
    if (id == g_unk_7172a092c8) return 2;
    if (id == g_unk_7172a092d0) return 3;
    if (id == g_unk_7172a092d8) return 4;
    if (id == g_unk_7172a092e0) return 5;
    if (id == g_unk_7172a092e8) return 6;
    if (id == g_unk_7172a092f0) return 7;
    if (id == g_unk_7172a092f8) return 8;
    if (id == g_unk_7172a09300) return 9;
    if (id == g_unk_7172a09308) return 10;
    if (id == g_unk_7172a09310) return 11;
    if (id == g_unk_7172a09318) return 12;
    if (id == g_unk_7172a09320) return 13;
    if (id == g_unk_7172a09328) return 14;
    if (id == g_unk_7172a09330) return 15;
    if (id == g_unk_7172a09338) return 16;
    if (id == g_unk_7172a09340) return 17;
    if (id == g_unk_7172a09348) return 18;
    if (id == g_unk_7172a09350) return 19;
    if (id == g_unk_7172a09358) return 20;
    if (id == g_unk_7172a09360) return 21;
    if (id == g_unk_7172a09368) return 22;
    if (id == g_unk_7172a09378) return 23;
    if (id == g_unk_7172a09380) return 24;
    return -1;
}
