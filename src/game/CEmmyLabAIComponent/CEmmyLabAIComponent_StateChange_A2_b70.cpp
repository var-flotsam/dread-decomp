// CEmmyLabAIComponent's unit (tools/tu_map.py): a state change from `from` to `to`. Staying in a state
// restarts fn_71004f7570 unless the record at +0x2a700 is marked; entering state 1 clears the mark and
// restarts; any other state marks the record once and saves the owner's (+0x30) position in it. Then the
// base fn_710054b93c. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "types.h"

struct UnkEmmyLabMark_A2_b70 {
    f32 x, y, z;
    bool marked;
};

struct UnkEmmyLabAI_A2_b70 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x2a700 - 0x38];
    UnkEmmyLabMark_A2_b70 mark;  // 0x2a700
};

void fn_71004f7570(UnkEmmyLabAI_A2_b70* self, bool flag);
void fn_710054b93c(UnkEmmyLabAI_A2_b70* self, s32 from, s32 to);

void fn_71004f7c08(UnkEmmyLabAI_A2_b70* self, s32 from, s32 to) {
    UnkEmmyLabMark_A2_b70* mark = &self->mark;
    if (from == to) {
        if (!mark->marked)
            fn_71004f7570(self, false);
    } else if (to == 1) {
        mark->marked = false;
        fn_71004f7570(self, false);
    } else if (!mark->marked) {
        mark->marked = true;
        UnkActor* owner = self->owner;
        f32 x = owner->field_0x44;
        f32 y = owner->field_0x54;
        f32 z = owner->field_0x64;
        mark->x = x;
        mark->y = y;
        mark->z = z;
    }
    fn_710054b93c(self, from, to);
}
