#pragma once

// CTunableMusicManager: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2637,
// 0x7100fd1c7c-0x7100fdeaa4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct UnkTuneMgrView_b0019;
struct UnkMusicMode;
struct UnkArray;
struct UnkMask;
struct UnkMgr_0xf18;
struct UnkMusicMgr_b0019;
struct UnkNode_0x68;
struct UnkRequest;
struct UnkMusicSlotOwner_fd8720;
// Applies `kind` under `mask` over `time` to every slot.
void fn_7100fd7b44(UnkMusicMgr_b0019* self, UnkMask kind, UnkMask mask, f32 time);
void fn_7100fdb9e0(UnkMgr_0xf18* manager, u32 kind, u32 flag, f32 a, f32 b);
void fn_7100fd7af8(void* manager, f32 level, f32 time);
void fn_7100fdcf2c(void* manager, u32 slot, bool active, f32 time);
f32 fn_7100fd7b3c(void* manager);
void fn_7100fd7c3c(UnkMgr_0xf18*, u32, UnkRequest*, f32, UnkMask, UnkMask);
void fn_7100fde4d8(UnkArray*, u32);
// Stops every active entry of slot `index` and empties it.
void fn_7100fd8720(UnkMusicSlotOwner_fd8720* self, u32 index);
void fn_7100fde6c4(UnkNode_0x68*);  // A node's destructor.
void fn_7100fd90d8(f32 a, f32 b, void* music, const base::global::CRntString* name, u32 c, const UnkMusicMode* d, u32 e, u32 f, void* g);
bool fn_7100fd76d4(void* manager, const base::global::CRntString* name);
extern char g_unk_7101cac1e0[];
f32 fn_7100fdc468(UnkTuneMgrView_b0019* manager, const u64* path);

// Applies `kind` under `mask` over `time` to every slot whose music is `name` (its key compared).
void fn_7100fd82dc(UnkMusicMgr_b0019* self, const base::global::CRntString* name, UnkMask kind, UnkMask mask,
                   f32 time);
bool fn_7100fd6f74(void* music, const char* name, const u32* mask);
void fn_7100fdbd84(UnkMgr_0xf18* self, u32 flag);
