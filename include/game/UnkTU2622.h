#pragma once

// UnkTU2622: functions of this translation unit that other files call (tools/tu_map.py unit 2622,
// 0x7100fc71ac-0x7100fc7f6c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct UnkPtrList;
struct UnkScenarioLoadRequest;
void fn_7100fc71ac(void* loader);  // The loader's constructor.

// Replaces the scenario load request with two entries, the first built from the arguments, the second
// from its own scenario and name only, and marks it pending (kind 6).
void fn_7100fc7500(UnkScenarioLoadRequest* self, const base::global::CRntString* scenario,
                   const base::global::CRntString* name, const base::global::CRntString* scenario2,
                   const base::global::CRntString* name2, UnkPtrList* blackboards, void (*fn)(),
                   const base::global::CRntString* project, const base::global::CRntString* s2,
                   const base::global::CRntString* s3);
