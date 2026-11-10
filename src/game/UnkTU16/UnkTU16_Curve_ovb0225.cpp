// Unit 16 (no reliable name): advances a timed curve. The time at +0x0 grows by the step up to the duration
// at +0x8 (nothing happens once it has reached it), and the value at +0x4 becomes what the curve function
// the table entry at +0xc holds gives for (0, 1, time / duration). Names and layouts are placeholders.
#include "types.h"

[[noreturn]] void fn_71000119d0();  // throws bad_function_call

struct UnkCurveFunction_b0225 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual f32 Call(const f32& a, const f32& b, const f32& t);
};

struct UnkCurveEntry_b0225 {
    u8 pad_0x0[0x20];
    UnkCurveFunction_b0225* function;  // 0x20
    u8 pad_0x28[0x8];
};
static_assert(sizeof(UnkCurveEntry_b0225) == 0x30, "entry layout");

extern UnkCurveEntry_b0225 g_unk_7101d4d0f0[] __attribute__((visibility("hidden")));

struct UnkTimedCurve_b0225 {
    f32 time;      // 0x0
    f32 value;     // 0x4
    f32 duration;  // 0x8
    s32 curve;     // 0xc
};

bool fn_7100011824(UnkTimedCurve_b0225* self, f32 step) {
    f32 time = self->time;
    f32 duration = self->duration;
    if (time != duration) {
        time += step;
        if (time > duration)
            time = duration;
        f32 ratio = time / duration;
        s32 index = self->curve;
        self->time = time;
        f32 zero = 0.0f;
        f32 one = 1.0f;
        f32 t = ratio;
        UnkCurveEntry_b0225* entry = &g_unk_7101d4d0f0[index];
        UnkCurveFunction_b0225* f = entry->function;
        if (!f)
            fn_71000119d0();
        self->value = f->Call(zero, one, t);
        return true;
    }
    return false;
}
