// Unit 108 of tools/tu_map.py (0x7100110c54-0x71001239b0): a bool property setter called through
// reflection: reads the bool out of a value holder (in place, or behind a pointer when the holder
// is in mode 1 or its type is wider than 8 bytes), stores it at +0x2c of the target and, when set,
// moves state 1 to 3 and 2 to 4 before fn_7100119c1c. Answers a 0x18-byte status {0, 0, 2, 1}.
// Names and layouts are placeholders.
#include "game/UnkTU108.h"
#include "types.h"

struct UnkValueType108 {
    u8 pad_0x0[0x18];
    u64 size;  // 0x18
};

struct UnkValue108 {
    union {
        u8 inline_data[8];
        u8* data;
    };
    UnkValueType108* type;  // 0x8
    u8 pad_0x10[0x4];
    u32 mode;  // 0x14
};

struct UnkTarget108 {
    u8 pad_0x0[0x2c];
    u8 enabled;  // 0x2c, a bool
    u8 pad_0x2d[0x13];
    u32 state;  // 0x40
};

struct UnkOwner108 {
    u8 pad_0x0[0x8];
    UnkTarget108* target;  // 0x8
};

struct UnkStatus108 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

UnkStatus108 fn_710011dd10(void* context, UnkOwner108* owner, UnkValue108* const* value) {
    UnkValue108* v = *value;
    UnkTarget108* target = owner->target;
    const u8* data = (v->mode == 1 || v->type->size >= 9) ? v->data : v->inline_data;
    u8 enabled = *data;
    target->enabled = enabled;
    if (enabled) {
        if (target->state == 2)
            target->state = 4;
        else if (target->state == 1)
            target->state = 3;
    }
    fn_7100119c1c(target);
    UnkStatus108 status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}
