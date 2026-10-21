// Sets an interpolated integer of the block (at +0x64) from a level converted through
// fn_710010aaa0, capped at 16000 (-1 also meaning 16000): with a zero time it jumps there and
// tells the listener the new value, otherwise it starts a transition from the current value.
#include "types.h"

struct UnkListener_7100105f94;

struct UnkInterp_7100105f94 {
    u8 pad_0x0[0x64];
    s32 current;
    s32 start;
    s32 target;
    f32 duration;
    f32 elapsed;
    UnkListener_7100105f94* listener;
    u8 pad_0x80[0x60];
    s32 field_0xe0;
};

s32 fn_710010aaa0(s32 level);
void fn_71000ffd4c(UnkListener_7100105f94* listener, UnkInterp_7100105f94* block, s32 value);

void fn_7100105f94(UnkInterp_7100105f94* self, s32 level, f32 time) {
    s32 value = fn_710010aaa0(level);
    if (self->current == value)
        return;
    s32 v = value == -1 ? 16000 : (value < 16000 ? value : 16000);
    if (time == 0.0f) {
        self->target = v;
        self->start = v;
        self->current = v;
        fn_71000ffd4c(self->listener, self, v);
        self->field_0xe0 = self->current;
    } else {
        self->start = self->current;
        self->target = v;
        self->duration = time;
        self->elapsed = 0.0f;
    }
}
