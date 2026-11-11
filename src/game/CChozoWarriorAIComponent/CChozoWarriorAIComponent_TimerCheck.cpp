// CChozoWarriorAIComponent's unit (tools/tu_map.py unit 733): unless `skip` is set, runs
// fn_71004996b0 once the timer at +0x14a1c reaches 0.2. Names are placeholders.
#include "game/CChozoWarriorAIComponent.h"
#include "types.h"

struct UnkChozoWarriorAI733 {
    u8 pad_0x0[0x14a1c];
    f32 field_0x14a1c;
};

void fn_710049acd8(UnkChozoWarriorAI733* self, void* context, s32 skip) {
    if (skip == 0 && self->field_0x14a1c >= 0.2f) {
        fn_71004996b0(self);
    }
}
