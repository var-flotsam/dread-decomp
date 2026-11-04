// Unit 50 of tools/tu_map.py (no reflected class): when the value is enabled (+0x9c) and active
// (+0x9d), starts a move to `target` over `time` (the target scaled by the last value when `scaled`):
// the current value (+0x0) becomes the start and the last value, the elapsed time is cleared, and
// unless +0xa3 is set the listener at +0x58 is told through its virtual at slot 0x28. Names and
// layouts are placeholders.
#include "types.h"

struct UnkUnit50Listener_b65a1 {
    void** vtable;
};

// A value copied as a raw word and read as a float.
union UnkUnit50Scalar_b65a1 {
    f32 v;
    u32 bits;
};

struct UnkUnit50Value_b65a1 {
    UnkUnit50Scalar_b65a1 value;
    UnkUnit50Scalar_b65a1 start;
    f32 target;
    f32 time;
    f32 elapsed;
    u8 pad_0x14[0x44];
    UnkUnit50Listener_b65a1* listener;  // 0x58
    u8 pad_0x60[0x1c];
    UnkUnit50Scalar_b65a1 last;  // 0x7c
    u8 pad_0x80[0x1c];
    bool enabled;  // 0x9c
    bool active;
    u8 pad_0x9e[0x5];
    bool quiet;  // 0xa3
};

void fn_7100072428(UnkUnit50Value_b65a1* self, bool scaled, f32 target, f32 time) {
    if (!self->enabled || !self->active)
        return;
    if (scaled)
        target = self->last.v * target;
    u32 bits = self->value.bits;
    self->target = target;
    self->time = time;
    self->elapsed = 0.0f;
    self->start.bits = bits;
    self->last.bits = bits;
    if (self->quiet)
        return;
    UnkUnit50Listener_b65a1* listener = self->listener;
    if (listener) {
        typedef void (*Fn)(UnkUnit50Listener_b65a1*, f32, UnkUnit50Value_b65a1*);
        f32 value;
        __builtin_memcpy(&value, &bits, sizeof(value));
        reinterpret_cast<Fn>(listener->vtable[0x28 / 8])(listener, value, self);
    }
}
