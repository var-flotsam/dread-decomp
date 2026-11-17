#pragma once

// UnkTU2562: functions of this translation unit that other files call (tools/tu_map.py unit 2562,
// 0x7100e1e35c-0x7100e31c18). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"
void fn_7100e211cc(void* self);
#include "base/UnkNoArg.h"
#include "base/global/CStrId.h"

struct UnkDropTarget_ovb0208;
// Hands the billboard singleton a drop request for `target`. The second argument register is never read
// (an UnkNoArg: the callers leave it unloaded).
void fn_7100e2c124(f32 value, UnkNoArg, UnkDropTarget_ovb0208* target, const base::global::CStrId* id, u32 a, u32 b,
                   u32 c, u32 d, u32 e, bool flag);
// The drops' callbacks, each given the manager.
void fn_7100e2f98c(void* owner);
void fn_7100e2f994(void* owner);
void fn_7100e2f99c(void* owner);
