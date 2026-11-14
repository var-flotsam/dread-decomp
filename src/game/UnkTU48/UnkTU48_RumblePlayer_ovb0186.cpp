// Unit 48 of tools/tu_map.py (the vibration player of src/base/input/UnkRumblePlayer.cpp, derived
// from nn::hid::VibrationPlayer): its constructor (the SDK base, the game's vtable, the two node
// connections, the fade and gain defaults, then the mutex) and the setter that stops or plays it
// and starts a fade towards the new state. Names and layouts are placeholders.
#include <new>

#include "base/UnkDefaultAllocator.h"
#include "base/input/UnkHidVibration.h"
#include "base/math/UnkZeroVector3.h"
#include "types.h"

extern char unk_7101a1a3a8[];

struct UnkRumbleVec_ovb0186 {
    f32 x, y, z;
};

// A mutex finalized on destruction (initialised in the player's constructor body).
struct UnkOwnedMutex_ovb0186 {
    nn::os::MutexType mutex;

    ~UnkOwnedMutex_ovb0186() { fn_7100066918(&mutex); }
};

struct UnkVibrationPlayerBase_ovb0186 : nn::hid::VibrationPlayer {
    UnkVibrationPlayerBase_ovb0186() { vtable = unk_7101a1a3a8 + 0x10; }
};

struct UnkVibrationPlayer_ovb0186 : UnkVibrationPlayerBase_ovb0186 {
    nn::hid::VibrationNodeConnection connection_0xf0;
    nn::hid::VibrationNodeConnection connection_0x128;
    f32 strength;                 // 0x160
    f32 from;                     // 0x164
    f32 to;                       // 0x168
    f32 duration;                 // 0x16c
    f32 elapsed;                  // 0x170
    UnkRumbleVec_ovb0186 offset;  // 0x174
    f32 balance;                  // 0x180
    f32 gain;
    f32 current;  // 0x188
    f32 frequency;
    f32 field_0x190;
    u8 field_0x194[0xf];
    bool dirty;  // 0x1a3
    bool field_0x1a4;
    u8 field_0x1a5[4];
    u8 pad_0x1a9[3];
    u32 field_0x1ac;
    UnkOwnedMutex_ovb0186 mutex;  // 0x1b0

    UnkVibrationPlayer_ovb0186() {
        elapsed = 0.0f;
        strength = 0.0f;
        from = 0.0f;
        to = 0.0f;
        duration = 0.0f;
        const u32* src = reinterpret_cast<const u32*>(&g_unk_7101647790);
        u32* dst = reinterpret_cast<u32*>(&offset);
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        balance = 1.0f;
        gain = 1.0f;
        current = 1.0f;
        frequency = 1.0f;
        field_0x190 = 1.0f;
        __builtin_memset(field_0x194, 0, sizeof(field_0x194));
        dirty = true;
        field_0x1a4 = true;
        field_0x1ac = 0;
        __builtin_memset(field_0x1a5, 0, sizeof(field_0x1a5));
        fn_71000668f8(&mutex.mutex);
    }
};

__attribute__((flatten)) void fn_7100070740(UnkVibrationPlayer_ovb0186* self) {
    new (self) UnkVibrationPlayer_ovb0186;
}

// The same player as the fade sees it: the strength is copied bitwise into the fade's start.
struct UnkRumbleFade_ovb0186 : nn::hid::VibrationPlayer {
    nn::hid::VibrationNodeConnection connection_0xf0;
    nn::hid::VibrationNodeConnection connection_0x128;
    u32 strength_bits;  // 0x160
    u32 from_bits;      // 0x164
    f32 to;             // 0x168
    f32 duration;       // 0x16c
    u32 elapsed;        // 0x170
    u8 pad_0x174[0x1a3 - 0x174];
    bool dirty;  // 0x1a3
    u8 pad_0x1a4[2];
    bool stopped;  // 0x1a6
};

// Stops or plays the player and, for a positive `duration`, starts a fade to the new state.
void fn_7100070e64(UnkRumbleFade_ovb0186* self, bool value, f32 duration) {
    if (self->stopped != value) {
        self->stopped = value;
        self->dirty = true;
        if (value) {
            if (duration <= 0.0f)
                self->Stop();
        } else {
            self->Play();
        }
        if (duration > 0.0f) {
            f32 target = value ? 0.0f : 1.0f;
            self->from_bits = self->strength_bits;
            self->to = target;
            self->duration = duration;
            self->elapsed = 0;
        }
    }
}
