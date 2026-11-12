#pragma once

// UnkTU2768: functions of this translation unit that other files call (tools/tu_map.py unit 2768,
// 0x710120320c-0x710120391c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct UnkPtrList;
struct CBlackboard;
struct UnkUserDataOwner;
void fn_710120320c(UnkUserDataOwner* self);  // Constructor.
void fn_7101203854(UnkUserDataOwner* self);
void fn_7101203768(UnkUserDataOwner* self);
s32 fn_7101203620(UnkUserDataOwner* self);
u32 fn_71012037dc(UnkUserDataOwner* self);
CBlackboard* fn_710120384c(UnkUserDataOwner* self);
s32 fn_71012034b0(UnkUserDataOwner* self, u32 kind, const base::global::CRntString* scenario, const base::global::CRntString* name, UnkPtrList* blackboards, bool a, void (*fn)(), bool b, const base::global::CRntString* c);
