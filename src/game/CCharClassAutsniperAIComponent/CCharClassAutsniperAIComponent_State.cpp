// CCharClassAutsniperAIComponent's unit (tools/tu_map.py unit 580): the state code of
// fn_710041b180 (CCharClassRobotAIComponent_State.cpp), overridden to 2 while the flag +0x179d of the
// second argument's +0x240 object is on. Names are placeholders.
#include "types.h"

struct UnkActor621;
u32 fn_710041b180(void* self, void* arg, UnkActor621* actor);

struct UnkInner580 {
    u8 pad_0x0[0x179d];
    bool field_0x179d;
};

struct UnkArg580 {
    u8 pad_0x0[0x240];
    UnkInner580* inner;  // 0x240
};

u32 fn_71003dbd9c(void* self, UnkArg580* arg, UnkActor621* actor) {
    u32 state = fn_710041b180(self, arg, actor);
    UnkInner580* inner = arg->inner;
    if (inner != nullptr) {
        state = inner->field_0x179d ? 2 : state;
    }
    return state;
}
