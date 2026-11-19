#pragma once

// CGoliathXBurstProjectionBombMovement: functions and globals of this translation unit that other files use (tools/tu_map.py unit 840,
// 0x7100589df0-0x710058cb00). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

namespace unk_static_init {
struct UnkFilePathStrIdBase;  // game_5821e4/UnkStaticInit.h
}  // namespace unk_static_init
struct UnkStatics_710058c890;
struct UnkPushedVector;
struct UnkValues3;
void fn_710058c768(UnkPushedVector* self, const UnkValues3* value);  // PushBack.
void fn_710058a2b8(void* self);
extern UnkStatics_710058c890 g_unk_71726e5cc0 __attribute__((visibility("hidden")));
// The effects' paths, as the unit's static initialiser builds them (fn_710058c890.cpp).
extern unk_static_init::UnkFilePathStrIdBase g_unk_71726e5cb0;  // the bomb effect's path
extern unk_static_init::UnkFilePathStrIdBase g_unk_71726e5cb8;  // the tracker effect's path
