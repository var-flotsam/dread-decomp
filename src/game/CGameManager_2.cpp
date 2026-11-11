// Whether the game manager's current start point (fn_7100e01680) has a smart-object component
// (fn_71006eb114, the typed lookup of "SMARTOBJECT").
#include "game/UnkTU1102.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkObj_7100e01680;  // src/game_aca57c/fn_7100e01680.cpp

bool fn_7100e01d74(UnkObj_7100e01680* self) {
    void* start = fn_7100e01680(self);
    return start && fn_71006eb114(start);
}
