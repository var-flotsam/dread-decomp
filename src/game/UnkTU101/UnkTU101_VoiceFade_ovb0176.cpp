// Unit 101 (no reflected class): a sound voice's fade/volume state. Names and layouts are placeholders.
#include "base/snd/UnkSoundSystemATK.h"
#include "game/UnkTU101.h"
#include "types.h"

using base::snd::UnkSoundSystemATK;

struct UnkSoundVoiceRef;

void fn_71000ffbb0(UnkSoundSystemATK* self, UnkSoundVoiceRef* voice, f32 value);
void fn_71000ffc24(UnkSoundSystemATK* self, UnkSoundVoiceRef* voice, f32 value);

union UnkBits_ovb0176 {
    f32 f;
    u32 u;
};

struct UnkVoiceState_ovb0176 {
    UnkBits_ovb0176 field_0x0;
    UnkBits_ovb0176 field_0x4;
    UnkBits_ovb0176 field_0x8;
    UnkBits_ovb0176 field_0xc;
    UnkBits_ovb0176 field_0x10;
    u8 unk_0x14[0x14];
    UnkBits_ovb0176 field_0x28;
    UnkBits_ovb0176 field_0x2c;
    UnkBits_ovb0176 field_0x30;
    u8 unk_0x34[8];
    UnkBits_ovb0176 field_0x3c;
    UnkBits_ovb0176 field_0x40;
    UnkBits_ovb0176 field_0x44;
    UnkBits_ovb0176 field_0x48;
    UnkBits_ovb0176 field_0x4c;
    UnkBits_ovb0176 field_0x50;
    UnkBits_ovb0176 field_0x54;
    UnkBits_ovb0176 field_0x58;
    UnkBits_ovb0176 field_0x5c;
    UnkBits_ovb0176 field_0x60;
    UnkBits_ovb0176 field_0x64;
    UnkBits_ovb0176 field_0x68;
    UnkBits_ovb0176 field_0x6c;
    u8 unk_0x70[8];
    UnkSoundSystemATK* owner;  // 0x78
    u64 field_0x80;
    u8 unk_0x88[0x10];
    f32 pos[3];  // 0x98
    u8 unk_0xa4[0x1c];
    s32 mode;  // 0xc0
    u32 voice_id;  // 0xc4
    u8 unk_0xc8[0x18];
    UnkBits_ovb0176 field_0xe0;
    UnkBits_ovb0176 field_0xe4;
    UnkBits_ovb0176 field_0xe8;
    UnkBits_ovb0176 field_0xec;
    UnkBits_ovb0176 field_0xf0;
    UnkBits_ovb0176 field_0xf4;
    UnkBits_ovb0176 field_0xf8;
    UnkBits_ovb0176 field_0xfc;
    u8 unk_0x100[8];
    UnkBits_ovb0176 field_0x108;
    u8 unk_0x10c[4];
    UnkBits_ovb0176 field_0x110;
    u8 unk_0x114[4];
    bool field_0x118;
    u8 unk_0x119[2];
    bool field_0x11b;
    u8 unk_0x11c[2];
    bool field_0x11e;
    bool field_0x11f;
    u8 field_0x120;
    bool field_0x121;
    bool field_0x122;
};
static_assert(__builtin_offsetof(UnkVoiceState_ovb0176, owner) == 0x78, "layout");
static_assert(__builtin_offsetof(UnkVoiceState_ovb0176, field_0x122) == 0x122, "layout");

static inline u32 Bits(f32 f) {
    UnkBits_ovb0176 b;
    b.f = f;
    return b.u;
}

static inline bool Near(f32 v, f32 w) {
    f32 lo = v - 0.00001f; f32 hi = v + 0.00001f; return (lo <= w) & (hi >= w);
}

void fn_7100105e40(UnkVoiceState_ovb0176* self, f32 a, f32 b) {
    u32 v;
    if (self->field_0xf8.f == a) {
        f32 c = self->field_0x50.f;
        if (Near(c, self->field_0x58.f))
            return;
        v = Bits(c);
    } else {
        v = self->field_0x50.u;
    }
    self->field_0x54.u = v;
    self->field_0x58.f = a;
    self->field_0x5c.f = b;
    self->field_0x60.f = 0.0f;
    self->field_0xf8.u = v;
    if (!self->field_0x121 && self->owner)
        fn_71000ffc24(self->owner, (UnkSoundVoiceRef*)self, a);
}

void fn_7100105da8(UnkVoiceState_ovb0176* self, f32 a, f32 b) {
    if (!self->field_0x11e) {
        if (self->field_0xf4.f == a && Near(self->field_0x3c.f, self->field_0x44.f))
            return;
        u32 v;
        if (b > 0.0f) {
            v = self->field_0x3c.u;
            self->field_0x40.u = v;
            self->field_0x44.f = a;
            self->field_0x48.f = b;
            self->field_0x4c.f = 0.0f;
        } else {
            v = Bits(a);
            self->field_0x3c.f = a;
            self->field_0x40.f = a;
            self->field_0x44.f = a;
        }
        u8 skip = self->field_0x121;
        self->field_0xf4.u = v;
        if (!skip && self->owner)
            fn_71000ffbb0(self->owner, (UnkSoundVoiceRef*)self, a);
    }
}
