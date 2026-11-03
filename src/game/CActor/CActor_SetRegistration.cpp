// CActor's unit (tools/tu_map.py unit 341): replaces the actor's registration (+0x1a8, as in
// src/game/CActor/CActor_ShutdownComponents.cpp) and, when it changed, tells the actor through its
// virtual slot 38 (+0x130), passing the old one. Names are placeholders.
#include "types.h"

struct UnkActor341_2;

struct UnkActor341_2Vtbl {
    void* slots_0x0[38];
    void (*on_registration_changed)(UnkActor341_2* self, void* old_registration);  // 0x130
};

struct UnkActor341_2 {
    UnkActor341_2Vtbl* vtable;
    u8 pad_0x8[0x1a0];
    void* registration;  // 0x1a8
};

void fn_71002337d0(UnkActor341_2* self, void* registration) {
    void* old_registration = self->registration;
    if (old_registration == registration) {
        return;
    }
    self->registration = registration;
    self->vtable->on_registration_changed(self, old_registration);
}
