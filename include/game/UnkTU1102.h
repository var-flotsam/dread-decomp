#pragma once

// UnkTU1102: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1102,
// 0x71006e5f24-0x71006ebf9c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/UnkRefHandle.h"
#include "types.h"

struct UnkActor;
struct UnkXParasiteSelect_9fcc;
struct UnkXParasiteAI;
struct UnkXParasiteGrab_xpa;
struct UnkXParasiteMode_A3_b75;
void fn_71006e9384(UnkXParasiteAI* self, const UnkXParasiteMode_A3_b75* mode);
void* fn_71006eb114(void* owner);  // refl_FindTyped of "SMARTOBJECT".
void fn_71006e892c(UnkXParasiteGrab_xpa* self);
void fn_71006ea3fc(UnkXParasiteAI* self, bool flag);
void* fn_71006eb1f0(void* h);
void fn_71006ea824(UnkXParasiteSelect_9fcc* self, UnkActor* target);
extern base::global::CStrId g_unk_717270b8e0 __attribute__((visibility("hidden")));
// Another copy of the handle assignment (base/UnkRefHandle.h), the one this unit's callback sets use.
base::UnkRefHandle* fn_71006eb8cc(base::UnkRefHandle* self, const base::UnkRefHandle* other);
