// CDoorLifeComponent's unit: opens the door (fn_71008d0ef4) or closes it (fn_71008d498c), as
// src/game/CDoorLifeComponent/CDoorLifeComponent_b44.cpp does. Names are placeholders.
#include "game/UnkTU1376.h"
#include "types.h"

struct UnkDoorLife_b44a2;

void fn_71008d51e4(UnkDoorLife_b44a2* self, bool open, bool quiet) {
    if (open) {
        fn_71008d0ef4(self);
        return;
    }
    fn_71008d498c(self, !quiet, 1, 1);
}
