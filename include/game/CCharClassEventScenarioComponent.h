#pragma once

// CCharClassEventScenarioComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1251,
// 0x71007dff10-0x71007e5b64). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkObj_71007e44b8;
struct UnkStruct_71007e44b8;
struct UnkProperty;
struct UnkEventScenario_b49a2;
struct UnkWeakTarget;
UnkWeakTarget* fn_71007e4924(void* owner);
void fn_71007e2ecc(UnkEventScenario_b49a2* self, bool a, bool b, f32 c, f32 d, f32 e, f32 f);
extern UnkProperty g_unk_717271f350;
UnkObj_71007e44b8* fn_71007e44b8(UnkStruct_71007e44b8* self, void* owner, const base::global::CStrId* name, u32 index);
