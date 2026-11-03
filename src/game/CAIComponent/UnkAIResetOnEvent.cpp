// CAIComponent unit (2702 in tools/tu_map.py): unless the event's flag at +0x8 is set, clears the
// counter at +0x113c and calls vtable slot 0x720 with (true, 0.0f); returns 0 either way.
// Names are placeholders.
#include "types.h"

struct UnkAIEvent {
    u8 pad_0x0[0x8];
    bool handled;  // 0x8
};

struct UnkAIResettable {
    void** vtable;
    u8 pad_0x8[0x1134];
    u32 field_0x113c;

    void vf_0x720(bool a, f32 b) { reinterpret_cast<void (*)(UnkAIResettable*, bool, f32)>(vtable[0x720 / 8])(this, a, b); }
};

// The signature src/game/CBehaviorTreeAIComponent/fn_710036974c.cpp calls it with.
struct UnkStruct_710036974c;
struct UnkStruct_710036974c_Msg;

u32 fn_7101111d88(UnkStruct_710036974c* self_, UnkStruct_710036974c_Msg* msg) {
    UnkAIResettable* self = reinterpret_cast<UnkAIResettable*>(self_);
    if (reinterpret_cast<const UnkAIEvent*>(msg)->handled)
        return 0;
    self->field_0x113c = 0;
    self->vf_0x720(true, 0.0f);
    return 0;
}
