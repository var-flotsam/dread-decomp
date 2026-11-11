#pragma once

// UnkTU182: functions of this translation unit that other files call (tools/tu_map.py unit 182,
// 0x710015f100-0x7100162a48). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGfxBuffer252;
struct UnkObj_7100161b90;
struct UnkRenderTargetSlot;
void fn_7100160990(s32 a, s32 b, bool c);
void fn_7100160ab0(u32 a, s32 b, u32 c);
void fn_7100161b90(void* obj);
void fn_710015f770(s32 slot, void* object, bool clear);
void fn_7100160d50(u32 a, s32 b, u32 c);
void fn_7100161d70(UnkGfxBuffer252* buffer);
void fn_7100161dc0(UnkGfxBuffer252* buffer);
void fn_7100161e60(UnkGfxBuffer252* buffer, u32 kind, u32 a, u32 offset, u32 format, const void* data, u32 size, u32 b);
u32 fn_71001622f0(u32 a, u32 b);
void fn_7100161800(void* object);
void fn_7100161960(UnkObj_7100161b90** out, u32 size, void* data, const u32* count);
void fn_7100161d60(UnkGfxBuffer252* buffer);

// Fills a target's description from its format, size and options.
void fn_710015f810(UnkRenderTargetSlot* out, u32 format, u32 width, u32 height, u32 a, void* b, void* c, u32 d, u32 e);

void fn_7100160590(u64 slot);
void fn_71001606f0(s32 a, s32 b, bool c);
void fn_7100160810(s32 a, s32 b, bool c);
void fn_7100160890(s32 a, s32 b, bool c);
void* fn_71001611e0(void* object);  // A target's NVN texture.
u32 fn_71001622e0(UnkGfxBuffer252* buffer);
