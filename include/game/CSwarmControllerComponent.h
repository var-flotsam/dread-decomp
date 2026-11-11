#pragma once

// CSwarmControllerComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1163,
// 0x71007469b0-0x710074eca4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMessage;
struct UnkSelf;
struct UnkSwarmController_A5_b68;
struct UnkTarget;
void fn_710074bdd4(UnkSwarmController_A5_b68* self, u32 index, f32 delay);

// Derived classes call it as their base's handler.
bool fn_710074ddd8(UnkSelf* self, UnkTarget* target, UnkMessage* message);

void fn_710074e190(void* self, void* effect, f32 value);
