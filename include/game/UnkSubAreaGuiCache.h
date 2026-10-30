#pragma once

// The 0x48-byte singleton a sub-area change creates on first use (g_unk_71727254f8,
// src/game/CSubAreaManager.cpp): unit 2734 of tools/tu_map.py, with the render pass list's slots
// layout (game/UnkRenderPassSlots.h). The name is a placeholder.

#include "game/UnkRenderPassSlots.h"

struct UnkSubAreaGuiCache : UnkRenderPassSlots {};

// The constructor (src/game/UnkUnit2734/fn_710119d484.cpp).
void fn_710119d484(UnkSubAreaGuiCache* self);
