#pragma once

// CFXComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2712,
// 0x710114d604-0x7101158410). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkPart;
struct UnkFxObject_ovb0041;
struct UnkObject;
struct UnkRinkaControl_ovb0090;
struct UnkRinkaControl_ruc;
struct UnkRinkaEntry_ruc;
struct UnkWorld;
bool fn_710114e680(UnkObject*, u8);
void fn_710115596c(UnkRinkaControl_ruc*, void*);
void fn_710114ffb8(UnkRinkaControl_ruc*, u64 hash, bool on, bool start);  // an effect by its name's hash
void fn_710115372c(UnkWorld* world);
char* fn_71011520a8(char* p0, u32 p1, char* p2, u32 p3);
void fn_7101152988(UnkRinkaControl_ovb0090* self, UnkFxObject_ovb0041* fx, UnkRinkaEntry_ruc** table, void* a, u32 b);
void fn_7101152c0c(UnkObject* self, void* effect, u32 on, u32 restart);
void fn_7101153c6c(void* object, void* effect);
extern void* g_unk_7101cb38e8[];
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);  // The part whose name has this hash, null when there is none.
void fn_710114fe8c(UnkWorld* world, u64 hash, u32 a, u32 b);
bool fn_710114e668(UnkObject*, u8);
