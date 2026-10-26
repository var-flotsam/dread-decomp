#pragma once

// A menu that keeps the handle of a composition it looked up through the GUI registry at
// `Offset`: the power description (0x488) and the mission menu (0x490) of the pause menus. The
// set-up functions (fn_7100b2c2bc, fn_7100b2cd24) are defined in src/game_aca57c/fn_7100b2c6a0.cpp.

#include "game/UnkGuiHandle.h"
#include "types.h"

template <u32 Offset>
struct UnkHandleMenu {
    u8 unk_0x0[Offset];
    UnkGuiHandle menu;
};

void fn_7100b2c2bc(UnkHandleMenu<0x488>* self);
void fn_7100b2cd24(UnkHandleMenu<0x490>* self);
