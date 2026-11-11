#pragma once

// CSoundSystem: functions of this translation unit that other files call (tools/tu_map.py unit 103,
// 0x7100106b64-0x7100108ea4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkPathNode;
struct UnkPlayingHandle_ovb0074;
struct UnkSoundDesc_ovb0074;
struct UnkSoundHandle;
struct UnkSoundMgr;
struct UnkSoundRequest_b50;
struct UnkSoundSystemPlayView_ovb0074;
void fn_710010808c(UnkSoundMgr* self, UnkPathNode* node);  // Takes a path.
void fn_7100108094(UnkSoundMgr* self, UnkPathNode* node);  // Releases a path.
void fn_710010806c(UnkSoundMgr* self, f32 amount);
UnkSoundHandle fn_71001080b0(UnkSoundMgr* mgr, const UnkSoundRequest_b50* request);
UnkPlayingHandle_ovb0074 fn_7100108620(UnkSoundSystemPlayView_ovb0074* self, UnkSoundDesc_ovb0074* desc);
void fn_7100108a98(UnkSoundMgr* mgr, u64 handle, bool flag, f32 value);
void fn_7100108b0c(UnkSoundMgr* self, u32 kind, f32 time);
