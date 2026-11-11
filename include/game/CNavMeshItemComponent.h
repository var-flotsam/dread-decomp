#pragma once

// CNavMeshItemComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1482,
// 0x71009bd8c0-0x71009c53ec). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

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
