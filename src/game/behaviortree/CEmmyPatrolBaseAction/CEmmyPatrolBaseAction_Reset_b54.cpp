// behaviortree::CEmmyPatrolBaseAction unit (tools/tu_map.py, the name from reflection): clears the flag at
// +0x11c, passes the given point to the member at +0x110 (fn_710007b444), and sets the wait at +0x118 to
// 1.5 plus a random factor in 0..1. Returns 0. Names and layouts are placeholders.
#include "game/UnkTU55.h"
#include "types.h"

extern "C" int rand();

struct UnkVec3;

struct UnkPatrolBase_b54 {
    u8 unk_0x0[0x110];
    u8 field_0x110[8];
    f32 wait;    // 0x118
    bool flag;   // 0x11c
};

u32 fn_7100516248(UnkPatrolBase_b54* self, const UnkVec3* point) {
    self->flag = false;
    fn_710007b444(self->field_0x110, point);
    self->wait = (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) + 1.5f;
    return 0;
}
