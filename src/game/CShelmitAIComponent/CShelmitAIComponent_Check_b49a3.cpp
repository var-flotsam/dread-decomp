// CShelmitAIComponent unit (tools/tu_map.py unit 1054). fn_71006bbf5c: unless told to skip, calls
// fn_71006baa5c(self, 0, 0) once the value at +0x14790 has reached the value at +0x620 of the target at
// +0x146c8. Names and layouts are placeholders.
#include "game/CShelmitAIComponent.h"
#include "types.h"

struct UnkShelmitTarget_b49a3 {
    u8 pad_0x0[0x620];
    f32 limit;  // 0x620
};

struct UnkShelmitState_b49a3 {
    UnkShelmitTarget_b49a3* target;  // 0x0
    u8 pad_0x8[0xc0];
    f32 value;  // 0xc8
};

struct UnkShelmitAI_b49a3 {
    u8 pad_0x0[0x146c8];
    UnkShelmitState_b49a3 state;  // 0x146c8
};

void fn_71006bbf5c(UnkShelmitAI_b49a3* self, void* arg, u32 skip) {
    UnkShelmitTarget_b49a3* target;
    // Expected not to call: the original places the call after the return.
    bool reached = skip == 0 && (target = self->state.target) != nullptr && self->state.value >= target->limit;
    if (__builtin_expect(reached, 0))
        fn_71006baa5c(self, 0, 0);
}
