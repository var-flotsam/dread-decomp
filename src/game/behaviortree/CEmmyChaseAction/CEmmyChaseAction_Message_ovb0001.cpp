// behaviortree::CEmmyChaseAction unit (tools/tu_map.py unit 801): fn_7100513cf0 handles a message (kind 1
// with the id hash 0xf4061ff25a2163d6): when the sender's id (+0x8, +0x10, +0x38) is the one the owner's
// CEmmyAIComponent keeps at +0x146c8 it calls fn_7100546654 on the component, otherwise it restarts the
// chase (fn_710051327c); senders with one of four ids then reset the action (fn_710051270c with 0).
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkEmmyChaseAction3_ovb0001;
bool fn_710051327c(UnkEmmyChaseAction3_ovb0001* self);
void fn_710051270c(UnkEmmyChaseAction3_ovb0001* self, f32 value);
struct UnkEmmyAI3_ovb0001;
void fn_7100546654(UnkEmmyAI3_ovb0001* ai);

struct UnkIdData_ovb0001 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkEmmyAI3_ovb0001 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x146c0];
    u64 field_0x146c8;
};

struct UnkChaseOwner3_ovb0001 {
    u8 pad_0x0[0x240];
    UnkEmmyAI3_ovb0001* ai;  // 0x240
};

struct UnkEmmyChaseAction3_ovb0001 {
    u8 pad_0x0[0x18];
    UnkChaseOwner3_ovb0001* owner;  // 0x18
};

struct UnkChaseSenderInfo_ovb0001 {
    u8 pad_0x0[0x38];
    UnkIdData_ovb0001* id;  // 0x38
};

struct UnkChaseSender_ovb0001 {
    u8 pad_0x0[0x10];
    UnkChaseSenderInfo_ovb0001* info;  // 0x10
};

struct UnkChaseMessage_ovb0001 {
    s32 kind;
    u8 pad_0x4[0x4];
    UnkChaseSender_ovb0001* sender;  // 0x8
    UnkIdData_ovb0001* id;  // 0x10
};

bool fn_7100513cf0(UnkEmmyChaseAction3_ovb0001* self, UnkChaseMessage_ovb0001* message) {
    if (message->id->hash != 0xf4061ff25a2163d6 || message->kind != 1)
        return true;
    UnkEmmyAI3_ovb0001* ai = self->owner->ai;
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        ai = nullptr;
    if (message->sender && message->sender->info->id->hash == ai->field_0x146c8)
        fn_7100546654(ai);
    else
        fn_710051327c(self);
    if (UnkChaseSender_ovb0001* sender = message->sender) {
        switch (sender->info->id->hash) {
        case 0xa62793ffe86c29a5:
        case 0x51c528eba89eee2e:
        case 0x7bf6fde1ddb95b8b:
        case 0x641c5e36ddd0abd8:
            fn_710051270c(self, 0.0f);
            break;
        }
    }
    return true;
}
