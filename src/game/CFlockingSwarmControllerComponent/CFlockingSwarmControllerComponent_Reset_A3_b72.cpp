// CFlockingSwarmControllerComponent unit (tools/tu_map.py, the name from reflection): fn_710073807c resets
// the controller after the base reset (fn_7100748424): no current index (+0x318 = -1), both member lists
// (+0x340, +0x368) resized (fn_7100738164) to the class's member count (+0xe8 -> +0x10 -> +0x110), every one
// of five flag bits set at +0x338 (a function-local "all" mask built from a function-local "none" one, both
// reached through the GOT like inline functions' statics) and the words at +0x3c0 and +0x2bc cleared.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSwarmFlags_A3_b72 {
    u32 bits;
    UnkSwarmFlags_A3_b72() : bits(0) {}
    explicit UnkSwarmFlags_A3_b72(u32 b) : bits(b) {}
};

inline const UnkSwarmFlags_A3_b72& SwarmFlagsNone_A3_b72() {
    static UnkSwarmFlags_A3_b72 none;
    return none;
}

inline const UnkSwarmFlags_A3_b72& SwarmFlagsAll_A3_b72() {
    static UnkSwarmFlags_A3_b72 all(~SwarmFlagsNone_A3_b72().bits & 0x1f);
    return all;
}

struct UnkSwarmClassData_A3_b72 {
    u8 pad_0x0[0x110];
    u32 member_count;  // 0x110
};

struct UnkSwarmClassHolder_A3_b72 {
    u8 pad_0x0[0x10];
    UnkSwarmClassData_A3_b72* data;  // 0x10
};

struct UnkFlockingSwarm2_A3_b72 {
    u8 pad_0x0[0xe8];
    UnkSwarmClassHolder_A3_b72* klass;  // 0xe8
    u8 pad_0xf0[0x1cc];
    u32 field_0x2bc;
    u8 pad_0x2c0[0x58];
    s32 current;  // 0x318
    u8 pad_0x31c[0x1c];
    UnkSwarmFlags_A3_b72 flags;  // 0x338
    u8 pad_0x33c[0x4];
    u8 list_0x340[0x28];
    u8 list_0x368[0x58];
    u32 field_0x3c0;
};

void fn_7100748424(UnkFlockingSwarm2_A3_b72* self);
void fn_7100738164(void* list, u32 count);

void fn_710073807c(UnkFlockingSwarm2_A3_b72* self) {
    fn_7100748424(self);
    self->current = -1;
    u32 count = self->klass->data->member_count;
    fn_7100738164(self->list_0x340, count);
    fn_7100738164(self->list_0x368, count);
    self->flags = SwarmFlagsAll_A3_b72();
    self->field_0x3c0 = 0;
    self->field_0x2bc = 0;
}
