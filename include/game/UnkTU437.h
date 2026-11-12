#pragma once

// UnkTU437: functions and globals of this translation unit that other files use (tools/tu_map.py unit 437,
// 0x710033fe10-0x710035962c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkActor;
struct UnkAimTunable;
struct UnkSelf;
void fn_7100359118(void* self);
void fn_7100350da0(UnkSelf*);
extern UnkAimTunable g_unk_71726b1438;
extern void* g_unk_71726b1688;  // The light manager.
extern void* g_unk_7101a23c58[];
extern void* g_unk_7101a23c80[];
void fn_7100352b14(void* body_field, UnkActor* owner, u32 flag);
