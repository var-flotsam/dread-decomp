#pragma once

// UnkTU2562: functions of this translation unit that other files call (tools/tu_map.py unit 2562,
// 0x7100e1e35c-0x7100e31c18). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"
void fn_7100e211cc(void* self);
#include "base/UnkNoArg.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"

struct UnkDropTarget_ovb0208;
struct UnkDropTunable_ovb0208;
// Hands the billboard singleton a drop request for `target`. The second argument register is never read
// (an UnkNoArg: the callers leave it unloaded).
void fn_7100e2c124(f32 value, UnkNoArg, UnkDropTarget_ovb0208* target, const base::global::CStrId* id, u32 a, u32 b,
                   u32 c, u32 d, u32 e, bool flag);
// The drops' callbacks (include/game/CDropLayout.h, CDrop's callback): each runs a query for the item and
// the argument. The first three pass the flag on; the last two pass 0 and add the item's position to the
// manager's running sum (src/game/CLogicDroppedItemManager/CLogicDroppedItemManager_Accumulate.cpp).
struct UnkDroppedItemManager;
struct UnkDroppedItem;
void fn_7100e2f98c(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg, bool flag);
void fn_7100e2f994(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg, bool flag);
void fn_7100e2f99c(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg, bool flag);
void fn_7100e2f9a4(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg, bool flag);
void fn_7100e2fa00(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg, bool flag);

// The billboards' path prefixes and suffixes (five strings, to 0x71727c14b8): fn_7100e21b8c puts names under
// the first two, fn_7100e2a734 between the first and each of the last three.
__attribute__((visibility("hidden"))) extern base::global::CRntString g_unk_71727c13c8[5];

// The billboard tunable's vtable (game/UnkDropTunable_ovb0208.h) and its destructor without the delete.
extern void* g_unk_7101aa9cc8[];
void fn_7100e20ba0(UnkDropTunable_ovb0208* self);
