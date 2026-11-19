#pragma once

// UnkTU2506: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2506,
// 0x7100d46804-0x7100d4bbfc). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkGroupArray_ovb0244;
struct UnkCollisionObject;
struct UnkManager_7100863a8c;
struct UnkStruct_71007e44b8;

extern char g_unk_7101aa87e8[];
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* entry);
void fn_7100d476e4(UnkManager_7100863a8c* self, void* obj);
void* fn_7100d475ec(UnkStruct_71007e44b8* self, const base::global::CStrId* name, u32 flag);
void fn_7100d46804(UnkManager_7100863a8c* self);
void fn_7100d4b0f4(UnkGroupArray_ovb0244* self, u32 capacity);
char* fn_7100d4b2fc(char* p0, char* p1);
