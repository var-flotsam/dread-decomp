// media::CMediaStream unit (tools/tu_map.py, the name from reflection): sets or clears bit `index`
// of the flags at +0x40 and, when the bit changes, tells the object through its virtual at slot
// 0x60. Names are placeholders.
#include "game/media/CMediaStream.h"
#include "types.h"

struct UnkMediaStream_b65a1 {
    void** vtable;
    u8 pad_0x8[0x38];
    u32 flags;
};

void fn_710021b860(UnkMediaStream_b65a1* self, u32 index, bool on) {
    u32 flags = self->flags;
    u32 bit = 1u << index;
    bool was = (flags & bit) != 0;
    self->flags = !on ? flags & ~bit : flags | bit;
    if (was != on) {
        typedef void (*Fn)(UnkMediaStream_b65a1*, u32, bool);
        reinterpret_cast<Fn>(self->vtable[0x60 / 8])(self, index, on);
    }
}
