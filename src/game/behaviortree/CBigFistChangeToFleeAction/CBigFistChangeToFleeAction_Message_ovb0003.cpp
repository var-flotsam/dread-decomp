// CBigFistChangeToFleeAction's unit: a message handler that raises the flag at +0x60 when the message
// (kind 1) carries the expected id. Names and layouts are placeholders.
#include "game/UnkBigFistAction.h"
#include "game/UnkBigFistMsg.h"
#include "game/UnkBigFistMsgData.h"
#include "game/UnkBigFistMsgInfo.h"
#include "types.h"

extern __attribute__((visibility("hidden"))) void* g_unk_71726c0b38;

bool fn_71003e3f5c(UnkBigFistAction* self, const UnkBigFistMsg* msg) {
    if (msg->kind == 1 && msg->data->field_0x10->field_0x38 == g_unk_71726c0b38)
        self->field_0x60 = true;
    return true;
}
