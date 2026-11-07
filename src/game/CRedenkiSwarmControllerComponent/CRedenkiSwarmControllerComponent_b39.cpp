// CRedenkiSwarmControllerComponent unit (tools/tu_map.py unit 977): a forwarder to fn_7100657478 that
// adds three of the object's own fields (+0x53c, +0x554, +0x318). Names and layouts are placeholders.
#include "types.h"

struct UnkRedenkiSwarm {
    u8 pad_0x0[0x318];
    u8 field_0x318[0x224];
    u8 field_0x53c[0x18];
    u8 field_0x554[4];
};

bool fn_7100657478(UnkRedenkiSwarm* self, void* a, void* b, void* c, void* d, void* e, void* f, void* g,
                   bool flag, f32 value);

// `value` (s0) reaches fn_7100657478 untouched in the original (tools/forwarded_args.py).
bool fn_7100657440(UnkRedenkiSwarm* self, void* a, void* b, void* c, void* d, bool flag, f32 value) {
    return fn_7100657478(self, a, b, c, d, self->field_0x53c, self->field_0x554, self->field_0x318, flag, value);
}
