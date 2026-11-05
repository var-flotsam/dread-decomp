// CDoorLifeComponent's unit: opens the door (fn_71008d0ef4) or closes it (fn_71008d498c), as
// src/game/CDoorLifeComponent/CDoorLifeComponent_b44.cpp does. Names are placeholders.
#include "types.h"

struct UnkDoorLife_b44a2;
bool fn_71008d0ef4(void*);
void fn_71008d498c(UnkDoorLife_b44a2* self, u32 a, u32 b, u32 c);

void fn_71008d51e4(UnkDoorLife_b44a2* self, bool open, bool quiet) {
    if (open) {
        fn_71008d0ef4(self);
        return;
    }
    fn_71008d498c(self, !quiet, 1, 1);
}
