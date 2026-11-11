// Message handlers in the CCharClassTunnelTrapMorphballComponent unit (tools/tu_map.py): when the
// message's type (msg->+0x18->+0x10->+0x20) is the one in g_unk_717274a6d8, run fn_7100a443c8. The
// second entry is the same handler reached through the base at +0x40. Names are placeholders.
#include "game/CCharClassTunnelTrapMorphballComponent.h"
#include "types.h"

struct UnkTunnelTrapMorphball;

struct UnkTunnelMsgType_b49a4 {
    u8 pad_0x0[0x20];
    void* type;
};
struct UnkTunnelMsgInfo_b49a4 {
    u8 pad_0x0[0x10];
    UnkTunnelMsgType_b49a4* field_0x10;
};
struct UnkTunnelMsg_b49a4 {
    u8 pad_0x0[0x18];
    UnkTunnelMsgInfo_b49a4* field_0x18;
};

extern void* g_unk_717274a6d8 __attribute__((visibility("hidden")));

bool fn_7100a446e4(u8* self, UnkTunnelMsg_b49a4* msg) {
    if (msg->field_0x18->field_0x10->type == g_unk_717274a6d8)
        fn_7100a443c8((UnkTunnelTrapMorphball*)self);
    return false;
}

bool fn_7100a44724(u8* self, UnkTunnelMsg_b49a4* msg) {
    if (msg->field_0x18->field_0x10->type == g_unk_717274a6d8)
        fn_7100a443c8((UnkTunnelTrapMorphball*)(self - 0x40));
    return false;
}
