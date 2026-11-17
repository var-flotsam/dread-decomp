// Unit 1428 (CMorphBallMovement's neighbours): keeps the ball's rolling loop sound for its mode. When
// the owner can play sounds and that loop is not playing, all four loops are stopped and cleared; while
// it plays, it is faded in, its pitch drops with time once the speed is high enough, and it follows
// the owner's position. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::math::CVector3D;

struct UnkInterp_7100105d34;
struct UnkPlayingSound_ovb0074;

// The playing sound behind a handle: its volume, pitch and whether it is fading in.
struct UnkLoopSound_l9459b8 {
    u8 pad_0x0[0x8];
    f32 volume;  // +0x8
    u8 pad_0xc[0x44 - 0xc];
    f32 pitch;  // +0x44
    u8 pad_0x48[0x11a - 0x48];
    bool fading;  // +0x11a
};

// The sound manager's reference block, as this file sees it: the sound and the block's serial.
struct UnkLoopSoundRef_l9459b8 {
    UnkLoopSound_l9459b8* sound;
    s64 serial;
};

static inline UnkLoopSoundRef_l9459b8* Ref(const UnkSoundHandle& handle) {
    return reinterpret_cast<UnkLoopSoundRef_l9459b8*>(handle.ref);
}
static inline bool Playing(const UnkSoundHandle& handle) {
    return Ref(handle) && Ref(handle)->serial == handle.serial && handle.serial != -1 && Ref(handle)->sound;
}
static inline UnkLoopSound_l9459b8* Get(const UnkSoundHandle& handle) {
    return Ref(handle) && Ref(handle)->serial == handle.serial && handle.serial != -1 ? Ref(handle)->sound : nullptr;
}

// v clamped to [min, max] (max first).
static inline f32 Clamp(f32 v, f32 min, f32 max) {
    f32 r = v >= max ? max : v;
    if (v <= min)
        return min;
    return r;
}

struct UnkBallActor_l9459b8 {
    u8 pad_0x0[0x38];
    f32 transform[12];  // +0x38, 3x4 row-major: the position is the last column
    u8 pad_0x68[0x2b8 - 0x68];
    void* audio;  // +0x2b8

    CVector3D GetPosition() const { return CVector3D{transform[3], transform[7], transform[11]}; }
};

// Stops a sound the actor plays (fn_7100d7c558, in CEntity's unit).
void fn_7100d7c558(UnkBallActor_l9459b8* actor, UnkSoundHandle* handle, f32 fade, f32 delay);
// Moves a playing sound to a position.
void fn_7100105ebc(UnkLoopSound_l9459b8* sound, const CVector3D* pos);

// The speed from which the loop's pitch drops, in the morph ball tunables (g_unk_71727393d8).
struct UnkBallTunable_l9459b8 {
    u8 pad_0x0[0x58];
    f32 pitch_speed;  // +0x58
};
extern UnkBallTunable_l9459b8 g_unk_71727393d8;

// The loops' sound names.
__attribute__((visibility("hidden"))) extern const CFilePathStrId g_unk_7172739478;
__attribute__((visibility("hidden"))) extern const CFilePathStrId g_unk_7172739480;
__attribute__((visibility("hidden"))) extern const CFilePathStrId g_unk_7172739488;
__attribute__((visibility("hidden"))) extern const CFilePathStrId g_unk_7172739490;

struct UnkBallMovement_l9459b8 {
    u8 pad_0x0[0x30];
    UnkBallActor_l9459b8* actor;  // +0x30
    u8 pad_0x38[0x2288 - 0x38];
    s32 mode;  // +0x2288
    u8 pad_0x228c[0x2320 - 0x228c];
    UnkSoundHandle idle_loop;  // +0x2320
    UnkSoundHandle field_0x2330;
    u8 pad_0x2340[0x10];
    UnkSoundHandle slow_loop;  // +0x2350
    UnkSoundHandle fast_loop;  // +0x2360
    u8 pad_0x2370[0x23c0 - 0x2370];
    f32 speed;  // +0x23c0
    u8 pad_0x23c4[0x254c - 0x23c4];
    bool fast;  // +0x254c
};

void fn_71009459b8(UnkBallMovement_l9459b8* self, f32 dt) {
    UnkSoundHandle loop;
    CFilePathStrId name;
    if (self->mode == 4) {
        if (self->fast) {
            loop = self->fast_loop;
            name = g_unk_7172739478;
        } else {
            loop = self->slow_loop;
            name = g_unk_7172739480;
        }
    } else if (self->mode == 2) {
        loop = self->field_0x2330;
        name = g_unk_7172739488;
    } else {
        loop = self->idle_loop;
        name = g_unk_7172739490;
    }

    UnkBallActor_l9459b8* actor = self->actor;
    if (actor->audio && !Playing(loop)) {
        fn_7100d7c558(actor, &self->fast_loop, 0.0f, 0.0f);
        fn_7100d7c558(self->actor, &self->slow_loop, 0.0f, 0.0f);
        fn_7100d7c558(self->actor, &self->field_0x2330, 0.0f, 0.0f);
        fn_7100d7c558(self->actor, &self->idle_loop, 0.0f, 0.0f);
        self->idle_loop = g_unk_7108b6c460;
        self->field_0x2330 = g_unk_7108b6c460;
        self->slow_loop = g_unk_7108b6c460;
        self->fast_loop = g_unk_7108b6c460;
    }

    if (Playing(loop) && Ref(loop)->sound->volume > 0.0f) {
        UnkLoopSound_l9459b8* sound = Ref(loop)->sound;
        if (!sound->fading)
            fn_7100105c9c(reinterpret_cast<UnkPlayingSound_ovb0074*>(sound), 0.1f);
        if (self->speed >= g_unk_71727393d8.pitch_speed) {
            f32 pitch = Clamp(Ref(loop)->sound->pitch - dt * 12.0f, 0.3f, 1.1f);
            fn_7100105da8(reinterpret_cast<UnkInterp_7100105d34*>(Get(loop)), pitch, 0.2f);
        }
        CVector3D pos = self->actor->GetPosition();
        fn_7100105ebc(Get(loop), &pos);
    }
}
