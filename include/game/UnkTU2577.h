#pragma once

// UnkTU2577: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2577,
// 0x7100edf654-0x7100ef3494). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "UnkLeftoverReg.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkMask;
struct UnkSubArea;
struct UnkTunable;
struct CSubAreaManager;
struct UnkIdSet;
struct UnkSubAreaChange_b190;
// The two settings steps take an empty value first: their callers leave x0 as it is.
struct UnkSubAreaTag : UnkLeftoverReg {};
// Hands the sub-area's settings for the slots set in `mask` to the environment manager.
void fn_7100eea4c8(UnkSubAreaTag, UnkSubArea* area, bool a, bool b, const UnkMask& mask);
// Hands the sub-area's camera settings to the main camera's controller.
void fn_7100eea618(UnkSubAreaTag, UnkSubArea* area, bool flag);
void fn_7100eeaa48(CSubAreaManager* self, const base::global::CStrId& prev_a, const base::global::CStrId& prev_b,
                   const base::global::CStrId& cur_a, const base::global::CStrId& cur_b, bool flag);
void fn_7100ee6ae4(CSubAreaManager* self);          // takes the player as the follow actor
void fn_7100ee4d34(CSubAreaManager* self, f32 dt);  // steps the sub-area change by its state
void fn_7100eeaed0(CSubAreaManager* self);          // enters the current sub-area
void fn_7100ee1548(void* self);  // An array's destructor (its callers register it).
void fn_7100ee8044(CSubAreaManager* self, void* prev, void* cur, UnkIdSet* set);
bool fn_7100eef674(void*);
void fn_7100ef1608(void* set, const UnkSubAreaChange_b190* change);
extern UnkTunable g_unk_71729da878;
// The sub-area manager's constructor (0xae8 bytes; the singleton is created on first use).
void fn_7100ee1f34(void* self);
