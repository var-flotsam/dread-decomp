#pragma once

// CBehaviorTreeAIComponent: functions of this translation unit that other files call (tools/tu_map.py unit 447,
// 0x71003690cc-0x710036a4f0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkComponent;
struct UnkNode;
struct UnkSclawkAI_b44a2;
struct UnkSclawkMessage_b44a2;
struct UnkWarriorSend_b58;
void fn_7100369380(void*);  // The base AI component's destructor.
u32 fn_710036974c(UnkSclawkAI_b44a2* self, UnkSclawkMessage_b44a2* msg);
void fn_710036932c(UnkComponent* self);
void fn_7100369d80(UnkWarriorSend_b58* self, void* arg);
void fn_71003695fc(UnkNode*);
