// The translation unit of engine::scene::CSceneSlot (its reflection and static initialiser are in
// src/game_s1/fn_7100205820.cpp; src/game_s1/fn_71001fcd4c.cpp finds slots by id and forwards to
// their setters). A slot's flag at 0x140, which it also passes on to the object at 0xa0 (its
// flag at 0x158). Layouts are placeholders.

#include "types.h"

// A slot, as src/game_s1/fn_71001fcd4c.cpp declares its setters' argument.
struct UnkItem;

struct UnkSceneSlotTarget {
    u8 pad_0x0[0x158];
    bool field_0x158;
};

struct UnkSceneSlot {
    u8 pad_0x0[0xa0];
    UnkSceneSlotTarget* field_0xa0;
    u8 pad_0xa8[0x98];
    bool field_0x140;
};
static_assert(__builtin_offsetof(UnkSceneSlot, field_0x140) == 0x140, "UnkSceneSlot layout");

void fn_7100202d44(UnkItem* item, bool value) {
    UnkSceneSlot* self = reinterpret_cast<UnkSceneSlot*>(item);
    self->field_0x140 = value;
    if (self->field_0xa0)
        self->field_0xa0->field_0x158 = value;
}
