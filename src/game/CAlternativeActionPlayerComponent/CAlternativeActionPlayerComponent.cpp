// In the CAlternativeActionPlayerComponent unit: when the component accepts the request (vtable
// slot 0x168) and its target at +0x10 (slot 0x170), hands the target and the two floats after it
// to fn_7100711fe8. Names and layouts are placeholders.
#include "types.h"

struct UnkAltActionTarget {
    u8 data[0x8];
    f32 field_0x8;
    f32 field_0xc;
};

struct UnkAltActionRequest {
    u8 pad_0x0[0x10];
    UnkAltActionTarget target;
};

struct UnkAltActionPlayer {
    void** vtable;
};

void fn_7100711fe8(UnkAltActionPlayer* self, UnkAltActionTarget* target, f32 a, f32 b);

void fn_7100711f78(UnkAltActionPlayer* self, UnkAltActionRequest* request) {
    typedef bool (*Accept)(UnkAltActionPlayer*, void*);
    if (reinterpret_cast<Accept>(self->vtable[0x168 / 8])(self, request)) {
        UnkAltActionTarget* target = &request->target;
        if (reinterpret_cast<Accept>(self->vtable[0x170 / 8])(self, target))
            fn_7100711fe8(self, target, target->field_0x8, target->field_0xc);
    }
}
