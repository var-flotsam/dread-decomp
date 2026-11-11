// CFixedCameraCtrl unit (tools/tu_map.py unit 1252): fn_71007e5e18 runs the base step (fn_71010ed98c),
// then takes the camera's position and second vector from its tuning (the object at +0x10 of +0xb8)
// or, without one, the position of the argument's node (+0x20) and the vector at +0xb4 of its
// definition (+0x130); each goes to its current (+0x68, +0x74) and start (+0x38, +0x44) slots. Names
// and layouts are placeholders.
#include "game/CCameraCtrl.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkTuning_fc {
    u8 pad_0x0[0x24];
    UnkVec3f pos;  // 0x24
    UnkVec3f dir;  // 0x30
};

struct UnkTuningRef_fc {
    u8 pad_0x0[0x10];
    UnkTuning_fc* tuning;
};

struct UnkDef_fc {
    u8 pad_0x0[0xb4];
    UnkVec3f dir;
};

struct UnkArg_fc {
    u8 pad_0x0[0x20];
    UnkXformNode node;  // 0x20
    UnkDef_fc* def;  // 0x130
};

struct UnkFixedCamera_fc {
    u8 pad_0x0[0x38];
    UnkVec3f start_pos;  // 0x38
    UnkVec3f start_dir;  // 0x44
    u8 pad_0x50[0x18];
    UnkVec3f pos;  // 0x68
    UnkVec3f dir;  // 0x74
    u8 pad_0x80[0x38];
    UnkTuningRef_fc* tuning;  // 0xb8
};

void fn_71007e5e18(UnkFixedCamera_fc* self, UnkArg_fc* arg) {
    fn_71010ed98c(self, arg, 1);
    if (self->tuning && self->tuning->tuning) {
        self->pos = self->tuning->tuning->pos;
        self->start_pos = self->pos;
        self->dir = self->tuning->tuning->dir;
    } else {
        self->pos = fn_7100206154(&arg->node);
        self->start_pos = self->pos;
        self->dir = arg->def->dir;
    }
    self->start_dir = self->dir;
}
