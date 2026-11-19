#pragma once

// CPlayerInfo: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2592,
// 0x7100f06628-0x7100f0fdb8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkPlayerInfo_ovb0071;
struct UnkEventTable_b0188;
struct UnkTimedEntry_ovb0071;
bool fn_7100f0ca8c(void* member);
void fn_7100f0e85c(UnkTimedEntry_ovb0071* self);
void fn_7100f0e98c(void*, void*, void*, void*);
extern void* g_unk_7101caa4b0[];
extern char g_unk_7101caa450[];
u32 fn_7100f0cc48(UnkEventTable_b0188* table, u32 value);
extern base::global::CFilePathStrId g_unk_71729db988 __attribute__((visibility("hidden")));  // the looping vibration file
void fn_7100f0e4e4(void* a, void* b, void* c, void* d);
void fn_7100f0dd3c(UnkPlayerInfo_ovb0071* self);
