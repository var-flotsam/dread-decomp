#pragma once

// CTunableMusicManager: functions of this translation unit that other files call (tools/tu_map.py unit 2637,
// 0x7100fd1c7c-0x7100fdeaa4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkArray;
struct UnkKind;
struct UnkLoadingMask;
struct UnkMask;
struct UnkMgr_0xf18;
struct UnkNode_0x68;
struct UnkRequest;
void fn_7100fd7b44(void* manager, UnkLoadingMask channels, UnkLoadingMask mask, f32 time);
void fn_7100fdb9e0(UnkMgr_0xf18* manager, u32 kind, u32 flag, f32 a, f32 b);
void fn_7100fd7af8(void* manager, f32 level, f32 time);
void fn_7100fdcf2c(void* manager, u32 a, u32 b, f32 value);
f32 fn_7100fd7b3c(void* manager);
void fn_7100fd7c3c(UnkMgr_0xf18*, u32, UnkRequest*, f32, UnkKind, UnkMask);
void fn_7100fde4d8(UnkArray*, u32);
void fn_7100fde6c4(UnkNode_0x68*);  // A node's destructor.
