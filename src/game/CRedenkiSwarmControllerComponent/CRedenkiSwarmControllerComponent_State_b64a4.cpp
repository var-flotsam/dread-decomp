// CRedenkiSwarmControllerComponent unit (tools/tu_map.py, the name from reflection): dispatches on
// the state at +0x530 to a virtual or a neighbour. Names are placeholders.
#include "types.h"

struct UnkRedenkiSwarm_b64a4 {
    void** vtable;
    u8 pad_0x8[0x528];
    u32 state;
};

void fn_7100650d74(UnkRedenkiSwarm_b64a4* self, f32 dt);
void fn_7100651168(UnkRedenkiSwarm_b64a4* self, f32 dt);

// Every state's update gets `dt` (s0, untouched in the original; tools/forwarded_args.py).
void fn_7100650d20(UnkRedenkiSwarm_b64a4* self, f32 dt) {
    typedef void (*Fn)(UnkRedenkiSwarm_b64a4*, f32);
    switch (self->state) {
    case 0:
        reinterpret_cast<Fn>(self->vtable[0x338 / 8])(self, dt);
        return;
    case 1:
        fn_7100650d74(self, dt);
        return;
    case 2:
        reinterpret_cast<Fn>(self->vtable[0x318 / 8])(self, dt);
        return;
    case 3:
        reinterpret_cast<Fn>(self->vtable[0x328 / 8])(self, dt);
        return;
    case 4:
        fn_7100651168(self, dt);
        return;
    }
}
