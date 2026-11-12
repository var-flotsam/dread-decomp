#pragma once

// UnkTU2611: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2611,
// 0x7100fa99bc-0x7100fadb10). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkMask.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkSubareaNamedEntry;
struct UnkSubareaNamedList;
struct UnkSubArea;
struct UnkSetupNamedDef;
struct UnkGeneratorRecord_ovb0120;
void fn_7100fa99bc(UnkGeneratorRecord_ovb0120* self);
void fn_7100fab18c(UnkGeneratorRecord_ovb0120* self, UnkGeneratorRecord_ovb0120* other);
// Applies `mask` to a sub-area (`c` is the other sub-area of a change, or null).
void fn_7100fac120(UnkSubArea* area, UnkMask mask, bool a, bool b, void* c, bool d);
extern UnkSetupNamedDef g_unk_71729ee018;  // The definition the setup chain is read under.
UnkSubareaNamedEntry* fn_7100fab838(UnkSubareaNamedList* self, const base::global::CStrId* id);
