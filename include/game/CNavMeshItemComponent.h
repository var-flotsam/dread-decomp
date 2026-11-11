#pragma once

// CNavMeshItemComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1482,
// 0x71009bd8c0-0x71009c53ec). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSourceDesc;
struct UnkOwner;
struct UnkSource;
struct UnkSourceList;
void fn_71009c0f1c(void* object, u64 id);
void fn_71009c172c(void* self);
void fn_71009bfda8(UnkOwner* self, UnkSource* src);
void fn_71009bffdc(UnkOwner* self, bool arg);
void fn_71009c0974(void*, bool);

// The element at `index`, growing the list when it is growable (base::UnkVector::GrowAt).
UnkSource*& fn_71009c0bf0(UnkSourceList* self, u32 index);
extern void* g_unk_7101a889c8[];

// The descriptor whose id fn_71009c28b8 looks for (a global pointer in .bss).
extern __attribute__((visibility("hidden"))) UnkSourceDesc* g_unk_717273ee10;

extern void* g_unk_7101a887d0[];  // The component's vtable (the second one at +0x198).
extern void* g_unk_7101a88970[];
