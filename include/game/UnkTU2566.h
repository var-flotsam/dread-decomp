#pragma once

// UnkTU2566: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2566,
// 0x7100e7cdfc-0x7100eb695c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkMinimapStatics;
struct UnkNamedDef_b43a3;
struct UnkMapData;
struct UnkMapTimersView_b43a3;
struct UnkMinimapOwner;
struct UnkMinimapRect;
struct UnkMmOwner_b0199;
struct UnkRecord;
struct UnkRecord118;
struct UnkRecordBuckets;
void fn_7100ea7034(UnkRecord118* self);
UnkMapData* fn_7100e8d8b0();
UnkRecord** fn_7100eb3814(UnkRecordBuckets* set, u32 bucket);
void fn_7100e94054(UnkMapTimersView_b43a3* self, f32 zoom);
UnkMinimapRect fn_7100e948b4(UnkMinimapOwner* self, bool flag);
void fn_7100e95038(void* world, void* object);
void fn_7100e950b4(void* world, void* object);
void fn_7100e951d4(UnkMinimapOwner* self, u32 count, void* items, u32 field_0x3);
void fn_7100e957a8(UnkMinimapOwner* self);
void fn_7100ea585c(UnkMmOwner_b0199* self);
void fn_7100ea5d7c(char*, void*);
void fn_7100ea7c6c(void*);
extern UnkNamedDef_b43a3 g_unk_71727c9f88;
extern bool g_unk_71727cc998 __attribute__((visibility("hidden")));
extern u8 g_unk_71727cc99c[] __attribute__((visibility("hidden")));
extern UnkMinimapStatics g_unk_71729cdaa0 __attribute__((visibility("hidden")));
void fn_7100e8f9a0(char* self, bool remove, base::global::CStrId* key);
extern base::global::CStrId g_unk_71727cc990;
void fn_7100e9c43c(UnkMapData* map, const base::global::CStrId* id);
void fn_7100e9cbac(void* world, const base::global::CStrId* current, base::global::CStrId* id, bool flag);
extern UnkTunable g_unk_71727ca410;
extern const f32 g_unk_710164be30;  // a margin of the minimap's cells (read only by minimap code)
