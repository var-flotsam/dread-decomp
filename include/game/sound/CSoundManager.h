#pragma once

// CSoundManager: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2646,
// 0x710101facc-0x710102cba4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

struct UnkAudioPreset_b0038;
struct UnkSoundHandle;
struct UnkStrIdPathPair;
struct UnkDropPlayer_ovb0208;
struct UnkDropTarget;
void fn_7101024b28(UnkDropTarget* self, void* record, void* source, void* a, u32 b);
void fn_7101023f04(UnkDropTarget* target, void* entry, UnkDropPlayer_ovb0208* player, void* arg4, u32 arg5);
extern void* g_unk_7101cac5d8[];
extern void* g_unk_7101cac688[];
extern void* g_unk_7101cac6e8[];
UnkSoundHandle fn_7101024e28(UnkDropTarget* self, const UnkStrIdPathPair* ids, void* a, u32 b);
void fn_710102be04(base::global::CRntVector<u64>* array);
bool fn_710102423c(UnkDropTarget* self, UnkAudioPreset_b0038* preset);
void fn_71010244a8(UnkDropTarget* self, UnkSoundHandle handle);
