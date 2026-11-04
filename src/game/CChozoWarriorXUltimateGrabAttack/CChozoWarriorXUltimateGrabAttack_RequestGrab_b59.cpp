// CChozoWarriorXUltimateGrabAttack's unit (tools/tu_map.py): event handlers that, when the event's id (event->+8->+0x10->+0x38->+0x30)
// is 0xa5b49f7b9da7fb4c, ask the attack's state machine (+0x80) for state 4 (RequestState_b58 of
// src/game/CSensorDoorComponent/UnkDoorStateMachine_b58.h, inlined). Names are placeholders.
#include "../CSensorDoorComponent/UnkDoorStateMachine_b58.h"
struct UnkGrabEventInfo_b59 {
    u8 pad_0x0[0x30];
    u64 id;
};
struct UnkGrabEventData_b59 {
    u8 pad_0x0[0x38];
    UnkGrabEventInfo_b59* info;
};
struct UnkGrabEventSrc_b59 {
    u8 pad_0x0[0x10];
    UnkGrabEventData_b59* data;
};
struct UnkGrabEvent_b59 {
    u64 field_0x0;
    UnkGrabEventSrc_b59* source;
};

template <u32 Offset>
struct UnkGrabAttack_b59 {
    u8 pad_0x0[Offset];
    UnkDoorStateMachine_b58 state_machine;
};

template <u32 Offset, u64 Hash>
static inline void OnGrabEvent_b59(UnkGrabAttack_b59<Offset>* self, UnkGrabEvent_b59* event) {
    if (event->source->data->info->id == Hash)
        RequestState_b58(&self->state_machine, 4);
}

void fn_7100489588(UnkGrabAttack_b59<0x80>* self, UnkGrabEvent_b59* event) {
    OnGrabEvent_b59<0x80, 0xa5b49f7b9da7fb4cull>(self, event);
}

void fn_7100489850(UnkGrabAttack_b59<0x80>* self, UnkGrabEvent_b59* event) {
    OnGrabEvent_b59<0x80, 0xa5b49f7b9da7fb4cull>(self, event);
}

