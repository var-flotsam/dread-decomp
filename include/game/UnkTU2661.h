#pragma once

// UnkTU2661: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2661,
// 0x71010626f0-0x7101073b80). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkCutsceneEntry_A5_b65;
struct UnkCutscene;
struct UnkCutsceneObject_cs;
struct UnkVec3_cs;
u32 fn_7101068c80(UnkCutsceneObject_cs* object, void* entry);
void fn_710106bcfc(UnkCutsceneObject_cs* object, const UnkVec3_cs& pos);
extern void* g_unk_7101cac988[];
void* fn_710106cf74(UnkCutsceneObject_cs* object, const base::global::CStrId* name);
void fn_710106838c(void* p);
void fn_710106b2bc(UnkCutscene* self, f32 frame);
UnkCutsceneEntry_A5_b65* fn_710106b284(void* object, u32 id);
extern void* g_unk_7101cac9b0[];
