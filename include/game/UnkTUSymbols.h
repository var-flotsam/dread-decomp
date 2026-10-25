#pragma once

// The game-side functions and globals that every unit's static initialiser names, declared once:
// the three function records a TU header carries, and the head of the global list of named
// property objects. Both helper families include this (include/game/UnkTUInit.h, the global one;
// src/game_5821e4/UnkStaticInit.h and src/game_s7/UnkStaticInitS7.h, the unk_static_init one), so a
// unit's own file never redeclares them. Layouts are placeholders.

#include "base/reflection/UnkReflTypeDtors.h"

void fn_710000cf00();
void fn_710000d470();
void fn_710000d4f4();

// A named property object (a global reached through the GOT) linked into a global list on
// construction; the list head is g_unk_71726db788. The full shapes are UnkTUInit.h's UnkProperty and
// UnkPropertyEx<> (UnkStaticInit.h's UnkProperty, UnkProperty2 and UnkProperty3 have the same layouts).
struct UnkPropertyHead {
    void** vtable;
    UnkPropertyHead* next;
};
extern UnkPropertyHead* g_unk_71726db788;  // the list head
