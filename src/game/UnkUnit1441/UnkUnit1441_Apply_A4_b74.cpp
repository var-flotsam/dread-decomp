// Unit 1441 of tools/tu_map.py: after the base fn_71009690a0, a message of kind 0 or 2 (+0x38) copies its four
// optional values (two words at +0x8/+0x10, two 12-byte values at +0x18/+0x28) into +0x98..+0xc4, after clearing
// all four first when `reset` is set. Names and layouts are placeholders.
#include "game/UnkTU1440.h"
#include "types.h"

namespace {

template <class T>
struct UnkOptional_A4_b74 {
    T value;
    bool has;

    void Assign(const UnkOptional_A4_b74& other) {
        if (other.has) {
            value = other.value;
            has = true;
        }
    }
};

struct UnkVec3_A4_b74 {
    f32 x, y, z;
};

}  // namespace

struct UnkUnit1441Msg_A4_b74 {
    u8 pad_0x0[0x8];
    UnkOptional_A4_b74<u32> field_0x8;
    UnkOptional_A4_b74<u32> field_0x10;
    UnkOptional_A4_b74<UnkVec3_A4_b74> field_0x18;
    UnkOptional_A4_b74<UnkVec3_A4_b74> field_0x28;
    u32 kind;  // 0x38
};

struct UnkUnit1441Apply_A4_b74 {
    u8 pad_0x0[0x98];
    UnkOptional_A4_b74<u32> field_0x98;
    UnkOptional_A4_b74<u32> field_0xa0;
    UnkOptional_A4_b74<UnkVec3_A4_b74> field_0xa8;
    UnkOptional_A4_b74<UnkVec3_A4_b74> field_0xb8;
};

void fn_710096953c(UnkUnit1441Apply_A4_b74* self, const UnkUnit1441Msg_A4_b74* msg, bool reset) {
    fn_71009690a0(self, msg, reset);
    if (msg->kind != 0 && msg->kind != 2)
        return;
    if (reset) {
        self->field_0x98.has = false;
        self->field_0xa0.has = false;
        self->field_0xa8.has = false;
        self->field_0xb8.has = false;
    }
    self->field_0x98.Assign(msg->field_0x8);
    self->field_0xa0.Assign(msg->field_0x10);
    self->field_0xa8.Assign(msg->field_0x18);
    self->field_0xb8.Assign(msg->field_0x28);
}
