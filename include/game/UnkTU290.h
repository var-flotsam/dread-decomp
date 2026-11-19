#pragma once

// UnkTU290: functions of this translation unit that other files call (tools/tu_map.py unit 290,
// 0x710020c38c-0x710020d0dc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkToggle;
struct UnkLightNode;
struct UnkMatrixNode;
struct UnkVec3f;
void fn_710020c9ac(void* self, f32 value);
void fn_710020c634(UnkLightNode* self);  // Passes the part's object on (fn_71001bd9f0).
void fn_710020c8ac(void*, bool);
void fn_710020c494(void* block, bool value);
void fn_710020c740(void* block, f32 value);
// Sets the translation of a node's matrix when it moved (src/game_s1/fn_710020ca64.cpp).
void fn_710020ca64(UnkMatrixNode* self, const UnkVec3f* v);
void fn_710020c4b8(UnkToggle* self, bool value);
