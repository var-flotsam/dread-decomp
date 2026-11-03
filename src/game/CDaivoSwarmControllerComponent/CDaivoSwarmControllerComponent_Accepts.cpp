// CDaivoSwarmControllerComponent's unit (tools/tu_map.py unit 768): whether a member counts: its flag
// +0x24 must be set; with the component's flag +0x588 set its +0x54 must also be nonzero, else the
// member may be null (no). A zero +0x54 is the expected case (it lays the "false" return out first, as
// in the original). Names are placeholders.
#include "types.h"

struct UnkSwarmMember768 {
    u8 pad_0x0[0x24];
    bool field_0x24;
    u8 pad_0x25[0x2f];
    s32 field_0x54;
};

struct UnkSwarmController768 {
    u8 pad_0x0[0x588];
    bool field_0x588;
};

bool fn_71004e25e8(const UnkSwarmController768* self, const UnkSwarmMember768* member) {
    if (self->field_0x588) {
        if (__builtin_expect(member->field_0x54 == 0, 1)) {
            return false;
        }
        return member->field_0x24;
    }
    return member != nullptr && member->field_0x24;
}
