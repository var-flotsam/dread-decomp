// Unit 248 of tools/tu_map.py (0x71001bc2dc-0x71001bcb3c): restarts the animation of the model at 0x78
// (fn_7100193350), marks the byte at 0x82, then looks up the unit's key g_unk_716e5547c0 in the model
// (fn_710019190c) and steps the model (fn_7100192f6c, by its step at +0x80) as many times as the first
// value of the pair found divided by the second. Names and layouts are placeholders.
#include "types.h"

// The definitions of these two are generated (src/lifted/); their declarations are copied from there.
char* fn_7100193350(char* p0);
u64 fn_710019190c(char* p0, char* p1);
void fn_7100192f6c(char* model, f32 step, bool flag);

extern char g_unk_716e5547c0[] __attribute__((visibility("hidden")));

struct UnkModel248 {
    u8 pad_0x0[0x80];
    f32 step;
};

struct UnkRatio248 {
    f32 num;
    f32 den;
};

struct UnkObject248 {
    u8 pad_0x0[0x78];
    UnkModel248* model;
    u8 pad_0x80[0x2];
    bool field_0x82;
};

void fn_71001bc634(UnkObject248* self) {
    fn_7100193350((char*)self->model);
    self->field_0x82 = true;
    UnkRatio248* ratio = (UnkRatio248*)fn_710019190c((char*)self->model, g_unk_716e5547c0);
    if (!ratio)
        return;
    u32 steps = (u32)(ratio->num / ratio->den);
    for (u32 i = steps; i != 0; --i)
        fn_7100192f6c((char*)self->model, self->model->step, true);
}
