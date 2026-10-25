#pragma once

// The AI component of an actor (the slot at +0x240 of game/UnkActor.h, whose class the callers
// test against the *AIComponent getters; tools/tu_map.py puts fn_71010fccf8 in the unit it names
// after CAIComponent) as the hand-written callers see it: a component with its owner actor at
// +0x30, like the other components of game_s2. Only the members the callers share are spelled
// out; the names and the layout are placeholders.

#include "game/UnkActor.h"
#include "types.h"

struct UnkAIComponent {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;  // the owner actor
};
static_assert(__builtin_offsetof(UnkAIComponent, field_0x30) == 0x30, "UnkAIComponent placeholder layout");

// The owner's slot at +0x298 as fn_71010fccf8's callers see it: the object that fn_71006f4b0c
// (game_s2/UnkParams.h, in the unit tools/tu_map.py names after CAINavigationComponent) takes
// parameter blocks for, and that three game_s2 files hand to it straight from the actor. Its
// +0x30 is the owner actor too; the callers read a record list through its +0xb0.
struct UnkNavigation {
    u8 pad_0x0[0xb0];
    void* field_0xb0;  // the record list: +0x0 of it is the list, +0x10 of that its first record
};
static_assert(__builtin_offsetof(UnkNavigation, field_0xb0) == 0xb0, "UnkNavigation placeholder layout");

// The navigation object of the AI component's owner: the actor's slot at +0x298 (null when the
// actor has none), reached through the component's owner pointer at +0x30. The definition is
// generated (src/lifted/).
UnkNavigation* fn_71010fccf8(UnkAIComponent* self);
