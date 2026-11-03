// Two functions in the CShelmitAIComponent unit that sample a float of the source object (+0x634 or
// +0x638) into the tracker at +0x146c8: the current value, and its rate of change over the time step;
// for a time step under 1 ms the rate is cleared and fn_71006badf4 restarts with the value. Names and
// layouts are placeholders.
#include "types.h"

// Declared as src/game/CShelmitAIComponent/CShelmitAIComponent.cpp declares it; it also takes the value.
struct UnkShelmitAI;
void fn_71006badf4(UnkShelmitAI* self);

struct UnkShelmitSource {
    u8 pad_0x0[0x634];
    f32 field_0x634;
    f32 field_0x638;
};
struct UnkShelmitTracker {
    UnkShelmitSource* source;
    u8 pad_0x8[0xcc];
    f32 previous;  // 0xd4
    f32 current;   // 0xd8
    f32 rate;      // 0xdc
};
struct UnkShelmitAI_b35 {
    u8 pad_0x0[0x146c8];
    UnkShelmitTracker tracker;
};

template <f32 UnkShelmitSource::*Field>
static inline __attribute__((always_inline)) void Sample(UnkShelmitAI_b35* self, f32 dt) {
    UnkShelmitTracker* t = &self->tracker;
    if (t->source == nullptr)
        return;
    f32 value = t->source->*Field;
    t->current = value;
    if (dt < 0.001f) {
        t->previous = value;
        t->rate = 0.0f;
        reinterpret_cast<void (*)(UnkShelmitAI_b35*, f32)>(fn_71006badf4)(self, value);
        return;
    }
    t->rate = (value - t->previous) / dt;
}

void fn_71006ba4a8(UnkShelmitAI_b35* self, f32 dt) { Sample<&UnkShelmitSource::field_0x638>(self, dt); }
void fn_71006bb908(UnkShelmitAI_b35* self, f32 dt) { Sample<&UnkShelmitSource::field_0x634>(self, dt); }
