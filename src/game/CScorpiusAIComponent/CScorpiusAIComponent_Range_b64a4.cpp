// CScorpiusAIComponent unit (tools/tu_map.py, the name from reflection): asks the virtual at slot
// 0x5e0 for two heights, makes them relative to the target's translation x (fn_7100d7c990 of the
// owner at +0x30) and lowers *a and *b to them. Names are placeholders.
#include "game/UnkActor.h"
#include "types.h"

struct UnkScorpiusRange_b64a4 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
};

void fn_71006977b8(UnkScorpiusRange_b64a4* self, void* arg1, f32* a, f32* b) {
    f32 hi = 0.0f;
    f32 lo = 0.0f;
    typedef void (*Fn)(UnkScorpiusRange_b64a4*, f32*, f32*);
    reinterpret_cast<Fn>(self->vtable[0x5e0 / 8])(self, &hi, &lo);
    f32 base = fn_7100d7c990(self->owner)->field_0x44;
    hi -= base;
    lo -= base;
    *a = *a < hi ? *a : hi;
    *b = *b < lo ? *b : lo;
}
