// CActor's unit (tools/tu_map.py unit 341): a test over the actor's registration flags: false when
// +0x1b5 is set or +0x1b4 is clear; else, when +0x1b0 is set, whether the actor has a
// registration (+0x1a8, as in CActor_ShutdownComponents.cpp), and true when it is clear. Names are
// placeholders.
#include "types.h"

struct UnkActorFlags341 {
    u8 pad_0x0[0x1a8];
    void* registration;  // 0x1a8
    bool field_0x1b0;
    u8 pad_0x1b1[0x3];
    bool field_0x1b4;
    bool field_0x1b5;
};

bool fn_7100233f48(const UnkActorFlags341* self) {
    if (self->field_0x1b5 || !self->field_0x1b4) {
        return false;
    }
    if (!self->field_0x1b0) {
        return true;
    }
    return self->registration != nullptr;
}
