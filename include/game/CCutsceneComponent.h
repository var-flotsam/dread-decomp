#pragma once

// CCutsceneComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1208,
// 0x71007a11ec-0x71007ae904). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkElem_0x88;
struct UnkCutsceneEntry_A5_b68;
void fn_71007a8280(void* object);
void fn_71007a8eec(void* self);
void fn_71007abba4(UnkCutsceneEntry_A5_b68* dst, UnkCutsceneEntry_A5_b68* src);
extern base::reflection::CType g_unk_7172719170;
UnkElem_0x88& fn_71007a6b58(base::UnkVector<UnkElem_0x88>* v, u32 index);
