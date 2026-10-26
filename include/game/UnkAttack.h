#pragma once

// The part of an attack that the subclasses' destructors and resets share. UnkAttackBase is
// CAttack's layout (game/CAttack.h, size 0x28) seen without its virtual destructor: the vtable
// as a raw pointer, the actor the attack belongs to (+0x8) and the key the attack registers with
// the actor's components (+0x18). The subclass views derive from it and pad from +0x20 (CAttack
// itself has a field there), so they cannot derive from CAttack directly.
// fn_71003609b8 is defined in src/game_lo/fn_71003609b8.cpp.

#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkAttackBase {
    void** vtable;
    UnkActor* field_0x8;
    u8 pad_0x10[0x8];
    u8 field_0x18[0x8];
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
static_assert(__builtin_offsetof(UnkAttackBase, field_0x8) == __builtin_offsetof(CAttack, field_0x8),
              "UnkAttackBase.field_0x8 is CAttack.field_0x8");
static_assert(__builtin_offsetof(UnkAttackBase, field_0x18) == __builtin_offsetof(CAttack, field_0x18),
              "UnkAttackBase.field_0x18 is CAttack.field_0x18");
static_assert(sizeof(UnkAttackBase) == __builtin_offsetof(CAttack, field_0x20),
              "UnkAttackBase ends where CAttack's last field begins");
#pragma clang diagnostic pop

// The part of the subclasses' destructors and resets that hands the attack's actor to the
// manager (g_unk_71726af468) when its AI component asks for it, then updates two other actor
// components.
void fn_71003609b8(UnkAttackBase* self);
