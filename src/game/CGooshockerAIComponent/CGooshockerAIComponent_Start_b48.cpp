// CGooshockerAIComponent unit (tools/tu_map.py): runs fn_710059e4f0(self, 1) once, clearing the
// counter at +0x148e4 first and setting the started flag at +0x146db after. Names and layouts are
// placeholders.
#include "game/CGooshockerAIComponent.h"
#include "types.h"

struct UnkGooshockerState {
    u8 pad_0x0[0x146db];
    bool started;  // 0x146db
    u8 pad_0x146dc[0x148e4 - 0x146dc];
    u32 field_0x148e4;
};

void fn_710059f058(UnkGooshockerState* self) {
    if (self->started)
        return;
    self->field_0x148e4 = 0;
    fn_710059e4f0(self, 1);
    self->started = true;
}
