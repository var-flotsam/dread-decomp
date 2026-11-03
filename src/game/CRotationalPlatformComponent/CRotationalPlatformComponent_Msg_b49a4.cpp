// Message handlers in the CRotationalPlatformComponent unit (tools/tu_map.py): when the message's
// type (msg->+0x18->+0x10->+0x20) is either of the two in g_unk_7172740990, run fn_71009d8bcc. The
// second entry is the same handler reached through the base at +0x40. Names are placeholders.
#include "types.h"

struct UnkRotPlatformMsgType_b49a4 {
    u8 pad_0x0[0x20];
    void* type;
};
struct UnkRotPlatformMsgInfo_b49a4 {
    u8 pad_0x0[0x10];
    UnkRotPlatformMsgType_b49a4* field_0x10;
};
struct UnkRotPlatformMsg_b49a4 {
    u8 pad_0x0[0x18];
    UnkRotPlatformMsgInfo_b49a4* field_0x18;
};

void fn_71009d8bcc(void* self);

extern void* g_unk_7172740990[2] __attribute__((visibility("hidden")));

bool fn_71009d8b90(u8* self, UnkRotPlatformMsg_b49a4* msg) {
    void* type = msg->field_0x18->field_0x10->type;
    if (type == g_unk_7172740990[0] || type == g_unk_7172740990[1])
        fn_71009d8bcc(self);
    return false;
}

bool fn_71009d8ee4(u8* self, UnkRotPlatformMsg_b49a4* msg) {
    void* type = msg->field_0x18->field_0x10->type;
    if (type == g_unk_7172740990[0] || type == g_unk_7172740990[1])
        fn_71009d8bcc(self - 0x40);
    return false;
}
