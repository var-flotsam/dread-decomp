// Unit 1444 of tools/tu_map.py (no name hint): the handler of track events (kind 1 and 2) of its
// class. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CActionInstance.h"
#include "game/UnkTU1444.h"
#include "types.h"

struct UnkTrackState_1444 {
    u8 pad_0x0[0x14];
    u32 phase;
};

struct UnkTrackAction_1444 {
    u8 pad_0x0[0x10];
    UnkTrackState_1444* state;
    u8 pad_0x18[0x48];
    f32 field_0x60;
};

struct UnkTrackEvent_1444 {
    u32 kind;
    UnkTrackAction_1444* action;
};

struct UnkUnit1444_Event {
    u8 pad_0x0[0x1fe];
    bool field_0x1fe;
    u8 pad_0x1ff[3];
    bool field_0x202;
    bool field_0x203;
    bool field_0x204;
    bool field_0x205;
};

void fn_71009746b8(UnkUnit1444_Event* self, UnkTrackEvent_1444* event) {
    switch (event->kind) {
    case 1:
        if (!self->field_0x1fe && !self->field_0x204 && event->action->state->phase != 2) {
            self->field_0x1fe = fn_7100971ee8(self);
            return;
        }
        if (event->action->state->phase == 0)
            self->field_0x203 = true;
        break;
    case 2: {
        if (self->field_0x205 || self->field_0x202)
            return;
        UnkTrackAction_1444* action = event->action;
        if (fn_7100c97930(action, fn_7100c0f988(), event->action->field_0x60))
            fn_7100974798(self, 0);
        break;
    }
    }
}
