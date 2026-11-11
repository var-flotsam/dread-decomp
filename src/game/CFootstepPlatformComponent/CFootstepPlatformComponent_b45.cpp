// CFootstepPlatformComponent's unit (tools/tu_map.py unit 1255): two event handlers of the same
// shape (fn_71007e88c4 the original). When the event's sender (+0x18, its object at +0x10, that
// object's id at +0x18) is the id (+0x30) of one of two unit statics, clears a flag and a counter;
// then sets the flag in front of them. Returns 0. Names and layouts are placeholders.
#include "game/CFootstepPlatformComponent.h"
#include "types.h"

struct UnkFootstepStatic_b45a2 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkFootstepSenderObject_b45a2 {
    u8 pad_0x0[0x18];
    u64 id;
};

struct UnkFootstepSender_b45a2 {
    u8 pad_0x0[0x10];
    UnkFootstepSenderObject_b45a2* object;
};

struct UnkFootstepEvent_b45a2 {
    u8 pad_0x0[0x18];
    UnkFootstepSender_b45a2* sender;
};

struct UnkFootstepState_b45a2 {
    bool active;
    u8 pad_0x1[7];
    bool flag;
    u8 pad_0x9[3];
    u32 count;
};

template <u32 Offset>
u32 UnkFootstepHandle_b45a2(u8* self, UnkFootstepEvent_b45a2* event) {
    auto* state = reinterpret_cast<UnkFootstepState_b45a2*>(self + Offset);
    u64 id = event->sender->object->id;
    if (g_unk_717271fae0->id == id || g_unk_717271fad8->id == id) {
        state->flag = false;
        state->count = 0;
    }
    state->active = true;
    return 0;
}

u32 fn_71007e88c4(u8* self, UnkFootstepEvent_b45a2* event) {
    return UnkFootstepHandle_b45a2<0xc8>(self, event);
}

u32 fn_71007e8910(u8* self, UnkFootstepEvent_b45a2* event) {
    return UnkFootstepHandle_b45a2<0x88>(self, event);
}
