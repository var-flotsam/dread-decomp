// Unit 101: sets a fade-time field and, when it changes, restarts the fade state if conditions allow.
// Names and layouts are placeholders.
#include "base/snd/UnkSoundSystemATK.h"

using base::snd::UnkSoundSystemATK;

struct UnkSoundVoice_b56;
void fn_71000ff768(UnkSoundSystemATK* self, UnkSoundVoice_b56* voice, f32 time);

struct UnkFade_l106038 {
    f32 start;  // 0x0
    f32 mid;  // 0x4
    f32 cur;  // 0x8
    f32 hold;  // 0xc
    f32 elapsed;  // 0x10
    u8 pad_0x14[0x78 - 0x14];
    UnkSoundSystemATK* owner;  // 0x78
    u8 pad_0x80[0xec - 0x80];
    f32 field_0xec;
    u8 pad_0xf0[0xfc - 0xf0];
    f32 field_0xfc;
    u8 pad_0x100[0x108 - 0x100];
    u32 field_0x108;
    u8 pad_0x10c[0x110 - 0x10c];
    f32 time;  // 0x110
    u8 pad_0x114[0x118 - 0x114];
    bool active;  // 0x118
    u8 pad_0x119[0x11b - 0x119];
    bool flag_0x11b;
    u8 pad_0x11c[0x121 - 0x11c];
    bool flag_0x121;
    bool flag_0x122;
};

void fn_7100106038(UnkFade_l106038* self, f32 time) {
    if (self->time > 0.0f && self->time != time) {
        bool was_active = self->active;
        self->time = time;
        if (was_active && !self->flag_0x122 && time <= 0.0f && !self->flag_0x11b) {
            f32 value = self->field_0xfc;
            if (!(time > 0.0f) && !self->flag_0x121 && self->owner)
                fn_71000ff768(self->owner, reinterpret_cast<UnkSoundVoice_b56*>(self), value);
            if (value > 0.0f) {
                f32 e = self->field_0xec;
                self->hold = value;
                self->elapsed = 0.0f;
                f32 s = self->start;
                self->mid = s;
                self->cur = e;
            }
            if (!self->active)
                self->field_0x108 = 0;
            self->active = true;
        }
    }
}
