#pragma once

// CCharacterMovement: functions of this translation unit that other files call (tools/tu_map.py unit 1445,
// 0x71009754a4-0x710097aa78). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGrappleTarget;
struct UnkAIReach;
struct UnkFlagged;
struct UnkMove_b152;
struct UnkPlace;
struct UnkShape;
struct UnkVec3Bits;
UnkShape* fn_7100976588(UnkFlagged* flagged);
bool fn_7100979ef0(void* tracked);
f32 fn_7100977b20(void*);
f32 fn_7100977bec(UnkFlagged*);
UnkAIReach fn_7100977b90(void* tracked);
void fn_7100978f58(char* self, const void* arg);  // Reads three words at `arg`.
void fn_71009796dc(UnkMove_b152* self);
UnkVec3Bits fn_7100979a60(UnkPlace* place);
bool fn_7100976c94(UnkGrappleTarget* target, const UnkVec3f* pos, u32 value, void* arg, bool flag, f32 offset);
