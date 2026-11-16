// CMorphBallMovement's unit (hint only): enters the morph ball: plays the timeline for the current mode,
// resets the movement's state, timers and position marks, and plays one of the two morph sounds, picked at
// random. Names and layouts are placeholders.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2772.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// The parameters a sound is played with (only the ids are released at exit).
struct UnkBallSoundParams {
    u32 field_0x0 = 0;
    u32 field_0x4 = 0;
    u32 field_0x8 = 0;
    u32 field_0xc = 0;
    u32 field_0x10 = 0;
    CStrId id = g_unk_7101d470e0;
    CFilePathStrId path = g_unk_7101d470e8;
    f32 field_0x28 = 0.0f;
    f32 field_0x2c = 0.0f;
    f32 field_0x30 = 0.0f;
    f32 field_0x34 = -1.0f;
    f32 field_0x38 = 1.0f;
    f32 field_0x3c = 500.0f;
    f32 field_0x40 = 2000.0f;
    f32 field_0x44 = 1.0f;
    f32 field_0x48 = 0.0f;
    f32 field_0x4c = 0.0f;
    f32 field_0x50 = 0.0f;
    f32 field_0x54 = -1.0f;
    f32 field_0x58 = -1.0f;
    f32 field_0x5c = -1.0f;
    f32 field_0x60 = 0.5f;
    f32 field_0x64 = 5.0f;
    f32 field_0x68 = 10.0f;
    bool field_0x6c = false;
    bool field_0x6d = false;
    bool field_0x6e = false;

    ~UnkBallSoundParams();  // fn_710034e0b8
};
static_assert(sizeof(UnkBallSoundParams) == 0x70);

struct UnkGroundMovement_ovb0138;
bool fn_71010d15ac(UnkGroundMovement_ovb0138* movement);

// The timelines played on entering, for each mode.
extern const u64 g_unk_710164b2f8;
extern const u64 g_unk_710164b300;
// The two morph sounds, built by the unit's static initialiser.
__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_7172739460;
__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_7172739468;

struct UnkBallMovement_l94ac7c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* actor;
    u8 pad_0x38[0xc1];
    u8 field_0xf9;
    u8 pad_0xfa[0xe6];
    u32 field_0x1e0;
    u8 pad_0x1e4[0x1c4];
    bool field_0x3a8;
    u8 pad_0x3a9[0x41f];
    u32 field_0x7c8;
    u8 pad_0x7cc[0x1be5];
    bool field_0x23b1;
    bool field_0x23b2;
    u8 pad_0x23b3[0x1];
    f32 field_0x23b4;
    u8 pad_0x23b8[0xc];
    f32 field_0x23c4;
    u8 pad_0x23c8[0x4c];
    f32 field_0x2414;
    f32 field_0x2418;
    f32 field_0x241c;
    f32 field_0x2420;
    f32 field_0x2424;
    u8 pad_0x2428[0x108];
    f32 field_0x2530;
    u8 pad_0x2534[0xd4];
    bool field_0x2608;
    u8 pad_0x2609[0x3];
    f32 field_0x260c;
    f32 field_0x2610;
    u8 pad_0x2614[0x30];
    bool field_0x2644;
    u8 pad_0x2645[0x3];
    f32 field_0x2648;

    void vf_0x220() { reinterpret_cast<void (*)(UnkBallMovement_l94ac7c*)>(vtable[0x220 / 8])(this); }
    void vf_0x260() { reinterpret_cast<void (*)(UnkBallMovement_l94ac7c*)>(vtable[0x260 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkBallMovement_l94ac7c, field_0x23b1) == 0x23b1);
static_assert(__builtin_offsetof(UnkBallMovement_l94ac7c, field_0x2414) == 0x2414);
static_assert(__builtin_offsetof(UnkBallMovement_l94ac7c, field_0x2608) == 0x2608);
static_assert(__builtin_offsetof(UnkBallMovement_l94ac7c, field_0x2648) == 0x2648);

// A random value in [lo, hi).
static inline f32 RandomRange(f32 lo, f32 hi) {
    f32 r = (f32)((f64)(fn_71012395a0() & 0x7fff) * (1.0 / 32767.0));
    return r * (hi - lo) + lo;
}

void fn_710094ac7c(UnkBallMovement_l94ac7c* self) {
    if (self->field_0x3a8) {
        UnkActor* actor = self->actor;
        bool alt = fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
        fn_7100d7c828(actor, alt ? &g_unk_710164b300 : &g_unk_710164b2f8, false);
    }
    if (self->field_0xf9 != 1) {
        self->field_0xf9 = 1;
        self->vf_0x260();
    }
    self->vf_0x220();
    self->field_0x23b1 = true;
    self->field_0x23b2 = true;
    self->field_0x23b4 = 0.0f;
    self->field_0x260c = 0.0f;
    self->field_0x2610 = 0.0f;
    self->field_0x7c8 = 0;
    self->field_0x1e0 = 0;
    self->field_0x2424 = g_unk_7101d47310->field_0x2444;
    UnkActor* actor = self->actor;
    self->field_0x241c = actor->field_0x44;
    self->field_0x2420 = 0.0f;
    self->field_0x2414 = actor->field_0x54;
    self->field_0x2418 = 0.0f;

    static UnkBallSoundParams s_params;
    s_params.field_0x0 = 0;
    s_params.field_0x4 = 9;
    s_params.field_0x6d = true;
    s_params.field_0x38 = 0.22f;
    s_params.field_0x3c = 5000.0f;
    s_params.field_0x40 = 20000.0f;
    s_params.field_0x44 = 1.5f;
    if (RandomRange(0.0f, 1.0f) > 0.5f)
        s_params.path = g_unk_7172739460;
    else
        s_params.path = g_unk_7172739468;
    fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->actor), &s_params, &g_unk_7101d470e0, false, true, true);

    if (self->field_0x2644) {
        self->field_0x2644 = false;
        self->field_0x2530 = 0.0f;
        self->field_0x2648 = -1.0f;
    }
    if (fn_71010d15ac(reinterpret_cast<UnkGroundMovement_ovb0138*>(self))) {
        self->field_0x2608 = true;
        self->field_0x23c4 = 0.25f;
    }
}
