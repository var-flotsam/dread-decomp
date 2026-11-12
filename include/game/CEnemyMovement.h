#pragma once

// CEnemyMovement: functions of this translation unit that other files call (tools/tu_map.py unit 2686,
// 0x71010c74dc-0x71010cef68). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkEnemyMovement_A5;
struct UnkEffectSink;
u32 fn_71010cc3a4(void*);
void fn_71010c7e6c(UnkEffectSink* sink, void* params, u32 flags);
void fn_71010cb2fc(void*, void*);
bool fn_71010cab0c(void* movement);
void fn_71010cb2f4(void* target, void* link);
void fn_71010cc214(UnkEffectSink* sink, u32 mode);
u32 fn_71010cc38c(void*);
UnkVec3f fn_71010c9ad0(UnkEnemyMovement_A5* self);
