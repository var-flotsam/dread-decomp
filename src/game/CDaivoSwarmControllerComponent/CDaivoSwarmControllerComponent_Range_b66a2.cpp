// CDaivoSwarmControllerComponent unit (tools/tu_map.py, the name from reflection): a distance by the mode
// at +0x390: 600, or in mode 0 the value at +0x828 of the object at +0x570 once +0x588 is set, in mode 2
// fn_710065aa30's, 0 past mode 3. Names and layouts are placeholders.
#include "types.h"

struct UnkDaivoSwarmParams_b66a2 {
    u8 pad_0x0[0x828];
    f32 range;
};

struct UnkDaivoSwarmController_b66a2 {
    u8 pad_0x0[0x390];
    u32 mode;
    u8 pad_0x394[0x1dc];
    UnkDaivoSwarmParams_b66a2* params;
    u8 pad_0x578[0x10];
    bool has_params;
};

f32 fn_710065aa30(UnkDaivoSwarmController_b66a2* self);

f32 fn_71004e1f48(UnkDaivoSwarmController_b66a2* self) {
    f32 range = 0.0f;
    switch (self->mode) {
    case 0:
        if (self->has_params) {
            range = self->params->range;
            break;
        }
    case 1:
    case 3:
        range = 600.0f;
        break;
    case 2:
        range = fn_710065aa30(self);
        break;
    }
    return range;
}
