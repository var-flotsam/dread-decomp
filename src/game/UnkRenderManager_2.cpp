// The render manager's unit (tools/tu_map.py: unit 315; include/game/UnkRenderManager.h): handing
// the object at +0x258 a factor kept within [0.15, 1] (fn_71001d0acc). Names are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

void fn_71001d0acc(void* object, f32 value);

void fn_710021a0c4(UnkRenderManager* self, f32 value) {
    f32 clamped;
    if (value <= 0.15f)
        clamped = 0.15f;
    else
        clamped = value >= 1.0f ? 1.0f : value;
    fn_71001d0acc(self->field_0x258, clamped);
}
