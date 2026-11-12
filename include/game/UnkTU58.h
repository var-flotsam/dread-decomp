#pragma once

// UnkTU58: functions and globals of this translation unit that other files use (tools/tu_map.py unit 58,
// 0x710007fdb4-0x71000815a4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

struct UnkGfxFrameSource;
struct UnkTimer_7100080c40;
void* fn_71000810b0(u32 value);
s64 fn_7100080b7c(UnkGfxFrameSource* timer);  // The timer's time in microseconds.
s64 fn_7100080fb4(const UnkTimer_7100080c40* self);
extern u32 g_unk_7101d58e48;  // The current frame.
void* fn_71000813e4(base::UnkSubAllocator* self, u64 size, u64 align, u32 flags);
void fn_7100081420(base::UnkSubAllocator* self, void* ptr);
