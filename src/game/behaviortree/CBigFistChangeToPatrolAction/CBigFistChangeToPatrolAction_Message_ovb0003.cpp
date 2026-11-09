// CBigFistChangeToPatrolAction's unit: a message handler that raises the flag at +0x60 when the message
// (kind 1) carries the expected id. Names and layouts are placeholders.
#include "types.h"

extern __attribute__((visibility("hidden"))) void* g_unk_71726c0ca0;

struct UnkBigFistMsgInfo {
    u8 pad_0x0[0x38];
    void* field_0x38;
};

struct UnkBigFistMsgData {
    u8 pad_0x0[0x10];
    UnkBigFistMsgInfo* field_0x10;
};

struct UnkBigFistMsg {
    s32 kind;
    UnkBigFistMsgData* data;  // 0x8
};

struct UnkBigFistAction {
    u8 pad_0x0[0x60];
    bool field_0x60;
};

bool fn_71003e4834(UnkBigFistAction* self, const UnkBigFistMsg* msg) {
    if (msg->kind == 1 && msg->data->field_0x10->field_0x38 == g_unk_71726c0ca0)
        self->field_0x60 = true;
    return true;
}
