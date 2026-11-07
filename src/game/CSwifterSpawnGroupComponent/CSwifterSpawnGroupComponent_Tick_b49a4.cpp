// CSwifterSpawnGroupComponent unit (tools/tu_map.py): after the base update (fn_710111bd1c or the
// bool-returning fn_710111c56c), counts the timer at +0x428 down to zero; when it is (or reaches)
// zero and the flag at +0x42c is clear, reloads +0x430 from +0x434. Names are placeholders.
#include "types.h"

struct UnkSwifterSpawnGroup_b49a4 {
    u8 pad_0x0[0x428];
    s32 timer;
    bool flag;
    u8 pad_0x42d[3];
    s32 field_0x430;
    s32 field_0x434;

    __attribute__((always_inline)) void Tick() {
        if (timer - 1 >= 0)
            timer = timer - 1;
        if (timer == 0 && !flag)
            field_0x430 = field_0x434;
    }
};

void fn_710111bd1c(UnkSwifterSpawnGroup_b49a4* self, void* a, void* b);
bool fn_710111c56c(UnkSwifterSpawnGroup_b49a4* self, void* a);

// The base updates get the callers' arguments (x1, x2), untouched in the original (tools/forwarded_args.py).
void fn_7100a131d0(UnkSwifterSpawnGroup_b49a4* self, void* a, void* b) {
    fn_710111bd1c(self, a, b);
    self->Tick();
}

bool fn_7100a13224(UnkSwifterSpawnGroup_b49a4* self, void* a) {
    bool r = fn_710111c56c(self, a);
    self->Tick();
    return r;
}
