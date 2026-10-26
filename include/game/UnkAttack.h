#pragma once

// The part of an attack (the root class is CAttack, size 0x28; src/game_5821e4/CAttack.h has the
// whole layout for the copy constructors) that the subclasses' destructors and resets share:
// the vtable, the actor the attack belongs to (+0x8) and the key the attack registers with the
// actor's components (+0x18). Subclasses derive from it and start their own members at +0x20.
// fn_71003609b8 is defined in src/game_lo/fn_71003609b8.cpp.

#include "game/UnkActor.h"
#include "types.h"

struct UnkAttackBase {
    void** vtable;
    UnkActor* field_0x8;
    u8 pad_0x10[0x8];
    u8 field_0x18[0x8];
};

// The part of the subclasses' destructors and resets that hands the attack's actor to the
// manager (g_unk_71726af468) when its AI component asks for it, then updates two other actor
// components.
void fn_71003609b8(UnkAttackBase* self);
