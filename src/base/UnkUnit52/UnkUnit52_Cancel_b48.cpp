// Unit 52 (tools/tu_map.py): cancels a pending request of an active object: in states 3 and 4 the
// completion callback (+0x20, with its context at +0x18) is told with no result and dropped; in
// state 2 it is only dropped; the state (+0xb8) then goes back to 0. Names and layouts are
// placeholders.
#include "types.h"

struct UnkUnit52Callback {
    void* context;
    void (*fn)(void* context, void* result);
};

struct UnkUnit52Request {
    bool active;
    u8 pad_0x1[0x17];
    UnkUnit52Callback callback;  // 0x18
    u8 pad_0x28[0x90];
    u32 state;                   // 0xb8
};

static inline void Cancel(UnkUnit52Callback* cb) {
    if (cb->fn != nullptr) {
        cb->fn(cb->context, nullptr);
        *cb = UnkUnit52Callback();
    }
}

struct UnkEngineObject_0xf0;

void fn_7100075e64(UnkEngineObject_0xf0* object, f32 dt) {
    UnkUnit52Request* self = reinterpret_cast<UnkUnit52Request*>(object);
    if (!self->active)
        return;
    switch (self->state) {
    case 3:
    case 4:
        Cancel(&self->callback);
        break;
    case 2:
        self->callback = UnkUnit52Callback();
        break;
    default:
        return;
    }
    self->state = 0;
}
