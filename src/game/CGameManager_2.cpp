// Whether the game manager's current start point (fn_7100e01680) has a smart-object component
// (fn_71006eb114, the typed lookup of "SMARTOBJECT").
#include "types.h"

struct UnkObj_7100e01680;  // src/game_aca57c/fn_7100e01680.cpp
void* fn_7100e01680(UnkObj_7100e01680* self);
void* fn_71006eb114(void* owner);  // refl_FindTyped of "SMARTOBJECT"

bool fn_7100e01d74(UnkObj_7100e01680* self) {
    void* start = fn_7100e01680(self);
    return start && fn_71006eb114(start);
}
