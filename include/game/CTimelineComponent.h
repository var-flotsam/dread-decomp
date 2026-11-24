#pragma once

// CTimelineComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1542,
// 0x7100a29e60-0x7100a2ec48). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/UnkCountedDeferredSet.h"
#include "types.h"

struct UnkTimelineComponent;

void fn_7100a2d4d0(void*, void*);
void fn_7100a2c200(UnkTimelineComponent* self, void* entry, __attribute__((noescape)) const u32* value);
void fn_7100a2cddc(void* table, void* key, base::global::CStrId* id, f32 delay, bool unique);
extern char g_unk_7101a8dcc0[];
extern char g_unk_7101a8de78[];
extern char g_unk_7101a8de98[];
void fn_7100a2e30c(base::UnkCountedDeferredSet<void>* self, void* value);
void CTimelineComponent_EndListenerWalk(base::UnkCountedDeferredWalk<void>* walk);
