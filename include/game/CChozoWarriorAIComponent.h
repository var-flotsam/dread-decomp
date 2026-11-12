#pragma once

// CChozoWarriorAIComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 733,
// 0x7100497b38-0x710049da84). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkChecker;
struct UnkChozoWarriorAI733;
struct UnkDescribed;
struct UnkSelf;
struct UnkWarriorTurn_b58;
void fn_7100498db8(UnkSelf* self);
void fn_71004996b0(UnkChozoWarriorAI733* self);
UnkDescribed* fn_710049b3cc(UnkChecker* self);
void fn_710049d094(UnkWarriorTurn_b58* self);
base::global::CStrId fn_710049c2f4(base::reflection::UnkReflObject* ai, f32 facing);
extern base::global::CStrId g_unk_71726d1270 __attribute__((visibility("hidden")));
