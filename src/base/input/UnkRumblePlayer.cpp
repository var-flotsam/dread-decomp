// From unit 48 of tools/tu_map.py (0x7100070740-0x71000710b0, no class hint): the game's vibration
// player (vtable unk_7101a1a3a8, derived from nn::hid::VibrationPlayer; its destructors are
// src/game_s1/fn_71000707dc.cpp), which fades its strength towards a target and feeds two node
// connections. Names and layouts are placeholders.
// Holds fn_7100070a28 (the per-frame update).
#include "base/input/UnkHidVibration.h"
#include "types.h"

#ifdef NNSDK
// The matching build's SDK headers leave the file header incomplete; the port's runtime defines it
// (port/runtime/include/nn/hid.h), with this layout.
namespace nn::hid {
struct VibrationFileInfo {
    u32 meta_data_size;
    u16 format_id;
    u16 sampling_rate;
    u32 data_size;
    s32 sample_length;
    s32 loop_start_position;
    s32 loop_end_position;
    s32 loop_interval;
};
}  // namespace nn::hid
#else
#include <nn/hid.h>
#endif

// A modulation as the connections take it: the low and high bands' amplitude and frequency.
struct UnkVibrationModulation {
    f32 amplitude_low;
    f32 frequency_low;
    f32 amplitude_high;
    f32 frequency_high;
};

struct UnkRumblePlayer : nn::hid::VibrationPlayer {
    nn::hid::VibrationNodeConnection connection_0xf0;
    nn::hid::VibrationNodeConnection connection_0x128;
    f32 strength;        // 0x160, fades from `from` to `to` over `duration`
    f32 from;            // 0x164
    f32 to;              // 0x168
    f32 duration;        // 0x16c
    f32 elapsed;         // 0x170
    u8 pad_0x174[0xc];
    f32 balance;         // 0x180, the share of the second connection
    f32 gain;            // 0x184
    f32 current;         // 0x188, the strength the modulations were last built from
    f32 frequency;       // 0x18c
    u8 pad_0x190[0xc];
    s32 position;        // 0x19c
    bool play_pending;   // 0x1a0
    bool stop_on_target; // 0x1a1, finish when the fade reaches its target
    bool finished;       // 0x1a2
    bool dirty;          // 0x1a3, the modulations need rebuilding
    bool field_0x1a4;
    u8 pad_0x1a5;
    bool stopped;        // 0x1a6
    u8 pad_0x1a7;
    bool field_0x1a8;
};
static_assert(__builtin_offsetof(UnkRumblePlayer, field_0x1a8) == 0x1a8, "UnkRumblePlayer placeholder layout");

// The two modulations, kept at +0xd0 and +0xe0 inside the SDK player's storage.
static inline UnkVibrationModulation* Modulations(UnkRumblePlayer* self) {
    return reinterpret_cast<UnkVibrationModulation*>(reinterpret_cast<u8*>(self) + 0xd0);
}

// The sampling step before the modulations are handed on (its second argument passed through).
void fn_7100070c40(UnkRumblePlayer* self, void* arg, f32 scale);

// Whether two strengths are within 1e-5 of each other.
static inline bool Near(f32 a, f32 b) { return a + 1e-5f >= b && a - 1e-5f <= b; }

static inline const nn::hid::VibrationModulation& AsModulation(const UnkVibrationModulation& m) {
    return reinterpret_cast<const nn::hid::VibrationModulation&>(m);
}

// Advances the fade by `dt`, rebuilds the modulations (scaled by `scale`) when the strength
// changed, and marks the player finished once it stops playing (a loop never does; a one-shot
// stops at its last sample or when it is back at the start).
// NON_MATCHING(minor): the same blocks laid out more compactly (the original keeps a separate exit
// for each early return and computes the end-of-file test without branches); registers differ.
void fn_7100070a28(UnkRumblePlayer* self, void* arg, f32 dt, f32 scale) {
    if (self->finished)
        return;
    f32 to = self->to;
    if (!Near(self->strength, to)) {
        f32 t = self->elapsed + dt;
        self->elapsed = t;
        f32 s;
        if (t < self->duration) {
            f32 k = t / self->duration;
            k = !(k >= 1.0f) ? k : 1.0f;
            s = self->from + k * (to - self->from);
            self->strength = s;
        } else {
            s = to;
            self->strength = to;
        }
        self->dirty = true;
        self->current = s;
        if (Near(s, to)) {
            if (self->stopped)
                self->Stop();
            if (self->stop_on_target)
                goto finish;
        }
    }
    {
        if (self->dirty) {
            UnkVibrationModulation* m = Modulations(self);
            f32 g = self->gain * scale * self->current;
            f32 f = self->frequency;
            m[0].frequency_high = f;
            m[0].frequency_low = f;
            m[1].frequency_high = f;
            m[1].frequency_low = f;
            f32 balance = self->balance;
            f32 a = g * (1.0f - balance);
            f32 b = g * balance;
            self->dirty = false;
            m[0].amplitude_high = a;
            m[0].amplitude_low = a;
            m[1].amplitude_high = b;
            m[1].amplitude_low = b;
            if (self->field_0x1a8)
                self->field_0x1a4 = true;
        }
        fn_7100070c40(self, arg, scale);
        self->connection_0xf0.SetModulation(AsModulation(Modulations(self)[0]));
        self->connection_0x128.SetModulation(AsModulation(Modulations(self)[1]));
        if (self->play_pending) {
            self->Play();
            self->play_pending = false;
            return;
        }
        self->position = self->GetCurrentPosition();
        bool loop = self->IsLoop();
        bool playing = self->IsPlaying();
        nn::hid::VibrationFileInfo info = self->GetFileInfo();
        if (self->stopped)
            return;
        if (playing && (loop || (self->position != info.sample_length && self->position != 0)))
            return;
    }
finish:
    self->finished = true;
}
