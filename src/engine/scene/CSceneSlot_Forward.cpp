// engine::scene::CSceneSlot: forwarding a request to the object at 0xa0 (fn_7100202d9c, the handler
// src/game_71001e2acc/fn_71001fd540.cpp calls), false when the slot has none. Layouts are
// placeholders.
#include "types.h"

struct UnkItem;
struct UnkSceneSlotObject;

struct UnkSceneSlotForward {
    u8 pad_0x0[0xa0];
    UnkSceneSlotObject* field_0xa0;
};

// The object's handler; the argument is passed by address.
bool fn_71001efa3c(UnkSceneSlotObject* object, bool a, void* const* arg, bool b);

bool fn_7100202d9c(UnkItem* item, bool a, void* arg, bool b) {
    UnkSceneSlotObject* object = reinterpret_cast<UnkSceneSlotForward*>(item)->field_0xa0;
    if (!object)
        return false;
    return fn_71001efa3c(object, a, &arg, b);
}
