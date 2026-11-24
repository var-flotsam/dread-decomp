#pragma once

// UnkTU182: functions and globals of this translation unit that other files use (tools/tu_map.py unit 182,
// 0x710015f100-0x7100162a48). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGfxTexture;
struct UnkObject_0x130;
struct UnkGfxHeap;
struct UnkPool_1587f0;
struct UnkGfxBuffer252;
struct UnkObj_7100161b90;
struct UnkRenderTargetSlot;
void fn_7100160990(s32 a, s32 b, u32);
void fn_7100160ab0(u32 a, s32 b, u32 c);
void fn_7100161b90(void* obj);
void SetGraphicsSlotPointer(s32 slot, void* object, u32);
void fn_7100160d50(u32 a, s32 b, u32 c);
void fn_7100161d70(UnkGfxBuffer252* buffer);
void fn_7100161dc0(UnkGfxBuffer252* buffer);
void fn_7100161e60(UnkGfxBuffer252* buffer, u32 kind, u32 a, u32 offset, u32 format, const void* data, u32 size, u32 b);
u32 fn_71001622f0(u32 a, u32 b);
void fn_7100161800(void* object);
void fn_7100161960(UnkObj_7100161b90** out, u32 size, void* data, const u32* count);
void fn_7100161d60(UnkGfxBuffer252* buffer);

// Fills a target's description from its format, size and options.
void FillRenderTargetSlotByFormat(UnkRenderTargetSlot* out, u32 format, u32 width, u32 height, u32 a, void* b, void* c, u32 d, u32 e);

void fn_7100160590(u64 slot);
void fn_71001606f0(u32 index, bool enable, u32 group);
void fn_7100160810(s32 a, s32 b, u32);
void fn_7100160890(s32 a, s32 b, u32);
void* fn_71001611e0(void* object);  // A target's NVN texture.
u32 GetBufferCapacityOrZero(UnkGfxBuffer252* buffer);
extern UnkPool_1587f0 g_unk_716e2a7580;  // The descriptors' memory.
extern UnkGfxHeap* g_unk_716e2a76a8;  // The heap for textures.
extern UnkGfxHeap* g_unk_716e2a76b0;  // The heap for render targets.
extern u32 g_unk_7101a1d814;
extern UnkGfxTexture* g_unk_716e2a3568[/* 0x800; unsized: x86-64 would assume 16-byte alignment */];
extern u32 g_unk_716e2a7568;
extern UnkObject_0x130* g_unk_716e2a76a0;
extern u8 g_unk_716e2a76e8[];
extern s32 g_unk_716e2a7708[/* 0x800; unsized: x86-64 would assume 16-byte alignment */];
void fn_7100160910(u32 index, s32 mode, u32 group);
