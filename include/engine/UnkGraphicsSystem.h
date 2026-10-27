#pragma once

// The graphics system: the 0x3183718-byte object behind g_unk_710b372a98 (layout and functions in
// src/engine/UnkGraphicsSystem.cpp, tools/tu_map.py unit 259), created by fn_7100150b70 and reached
// through the one-line wrappers of src/engine/UnkGraphicsApi.cpp (unit 167). Names are placeholders.

#include "types.h"

struct UnkGraphicsSystem;
// The display settings the creator passes on (all optional; src/engine/UnkGraphicsSystem.cpp).
struct UnkGfxSettings;

// The constructor.
void fn_71001c8294(UnkGraphicsSystem* self);
// The set-up. The creator passes a third argument in x2 that the set-up never reads.
void fn_71001c9718(UnkGraphicsSystem* self, UnkGfxSettings* settings, void* unused);
// Sets scale `index` (0 or 1), clamped to [0, 1]; false for another index.
bool fn_71001d2dcc(UnkGraphicsSystem* self, u32 index, f32 value);
