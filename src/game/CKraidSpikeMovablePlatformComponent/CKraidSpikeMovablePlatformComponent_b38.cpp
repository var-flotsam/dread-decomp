// CKraidSpikeMovablePlatformComponent: the per-frame update. Advances the timers (the one at +0x26c
// restarts unless its flag at +0x269 is set), moves from state 1 to 2 when the timer at +0x27c
// passes the platform's limit (storing the next delay, -1 meaning Kraid's tunable default), calls
// fn_7100608674 when the timer at +0x264 reaches its own limit (same default rule), then runs the
// base update. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

// CCharClassKraidAIComponent's tunable: the default delays at +0x24 and +0x28.
extern base::reflection::UnkTunable g_unk_71726f3440;

struct UnkKraidTunableDelays {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
};

struct UnkSpikePlatformConfig {
    u8 pad_0x0[0x78];
    f32 field_0x78;
    u8 pad_0x7c[4];
    f32 field_0x80;
    u8 pad_0x84[0x10];
    f32 field_0x94;
};

struct UnkSpikePlatform {
    u8 pad_0x0[0x250];
    UnkSpikePlatformConfig* config;  // 0x250
    f32 field_0x258;
    u8 pad_0x25c[4];
    bool field_0x260;
    u8 pad_0x261[3];
    f32 field_0x264;
    u8 pad_0x268;
    bool field_0x269;
    u8 pad_0x26a[2];
    f32 field_0x26c;
    u8 pad_0x270[0xc];
    f32 field_0x27c;
    u32 state;  // 0x280
};

void fn_7100608674(UnkSpikePlatform* self);
void fn_7100951f40(UnkSpikePlatform* self, f32 dt);

static inline const UnkKraidTunableDelays* Delays() {
    return reinterpret_cast<const UnkKraidTunableDelays*>(&g_unk_71726f3440);
}

void fn_7100608590(UnkSpikePlatform* self, f32 dt) {
    self->field_0x26c = self->field_0x269 ? self->field_0x26c + dt : 0.0f;
    self->field_0x269 = false;
    if (self->state == 1) {
        self->field_0x27c = self->field_0x27c + dt;
        UnkSpikePlatformConfig* config = self->config;
        if (self->field_0x27c > config->field_0x80) {
            self->state = 2;
            const UnkKraidTunableDelays* delays = Delays();
            f32 delay = config->field_0x78;
            f32 fallback = delays->field_0x24;
            self->field_0x258 = delay == -1.0f ? fallback : delay;
        }
    }
    if (self->field_0x260) {
        self->field_0x264 = self->field_0x264 + dt;
        const UnkKraidTunableDelays* delays = Delays();
        f32 limit = self->config->field_0x94;
        f32 fallback = delays->field_0x28;
        limit = limit == -1.0f ? fallback : limit;
        if (self->field_0x264 >= limit)
            fn_7100608674(self);
    }
    fn_7100951f40(self, dt);
}
