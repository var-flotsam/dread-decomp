// CBatalloonAIComponent unit (tools/tu_map.py, the name from reflection): returns 1 when the owner's (+0x30)
// resolved actor (fn_7100d7c990) has a +0x208 object that fn_71009e12d0 accepts; otherwise 2 when the request's
// s16 at +0x14 is -1 or its byte at +0x16 is 0xff, else 1. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "types.h"

bool fn_71009e12d0(void*);

struct UnkBatalloonRequest_b67a2 {
    u8 pad_0x0[0x14];
    s16 field_0x14;
    u8 field_0x16;
};

struct UnkBatalloonAIView_b67a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

u32 fn_71003de844(UnkBatalloonAIView_b67a2* self, UnkBatalloonRequest_b67a2* request) {
    UnkActor* actor = fn_7100d7c990(self->owner);
    if (actor != nullptr && actor->field_0x208 != nullptr && fn_71009e12d0(actor->field_0x208))
        return 1;
    if (request->field_0x14 == -1)
        return 2;
    return request->field_0x16 == 0xff ? 2 : 1;
}
