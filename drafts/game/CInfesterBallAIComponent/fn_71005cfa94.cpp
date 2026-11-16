// CInfesterBallAIComponent's unit (tools/tu_map.py, unit 898): the ball bursts once. It stops, sends the
// all-clear animation block, fires a shock wave at the owner's position sized by the tuning, sets the burst
// id on the owner's timeline and schedules the owner's removal. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CShockWaveComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

struct UnkInfesterBallKey_b60;
extern UnkInfesterBallKey_b60 g_unk_710164a120;  // reached through the GOT

extern CStrId g_unk_71726ecb60 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726ecb68 __attribute__((visibility("hidden")));

void fn_71009f51f8(char* self, void* owner);

// The kind-5 block with its two trailing flags set.
struct UnkParams5_l5cfa94 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 field_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

// The tunables block: the shock wave's sizes.
struct UnkInfesterTunables_l5cfa94 {
    u8 pad_0x0[0x43c];
    f32 field_0x43c;
};

struct UnkInfesterTuning_l5cfa94 {
    u8 pad_0x0[0x510];
    f32 field_0x510;
    f32 field_0x514;
    f32 field_0x518;
};

// The shock wave description fn_71009f35b0 builds and fn_71003cc474 destroys; its damage request at +0x8.
struct UnkInfesterShockWave_l5cfa94 {
    u8 pad_0x0[0x8];
    u8 request[0xb8];  // 0x8
    u8 id_raw[0x8];    // 0xc0: the request's id, a CStrId
    u8 pad_0xc8[0x44];
    u32 field_0x10c;
    u8 pad_0x110[4];
    CVector3D position;  // 0x114
    u8 field_0x120;
    u8 pad_0x121[3];
    f32 field_0x124;
    u8 pad_0x128[8];
    f32 field_0x130;
    f32 field_0x134;
    u8 pad_0x138[0x50];
    UnkWeakHandle* source;  // 0x188
    u8 field_0x190;
    u8 pad_0x191[0x1f];
    u8 name_raw[0x8];  // 0x1b0: a CStrId
    u8 pad_0x1b8[0x58];

    CStrId& Id() { return *reinterpret_cast<CStrId*>(id_raw); }
    CStrId& Name() { return *reinterpret_cast<CStrId*>(name_raw); }

    UnkInfesterShockWave_l5cfa94() { fn_71009f35b0(reinterpret_cast<char*>(this)); }
    ~UnkInfesterShockWave_l5cfa94() { fn_71003cc474(reinterpret_cast<UnkShockWaveMember*>(this)); }
};
static_assert(sizeof(UnkInfesterShockWave_l5cfa94) == 0x210, "shock wave layout");

// The ball's state at +0x146d0.
struct UnkInfesterState_l5cfa94 {
    UnkInfesterTuning_l5cfa94* tuning;  // 0x0
    u8 pad_0x8[0x2c];
    bool burst;  // 0x34
    u8 pad_0x35[3];
    u32 state;  // 0x38
    u8 pad_0x3c[2];
    bool field_0x3e;
};

struct UnkInfesterBallAI_l5cfa94 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x146d0 - 0x38];
    UnkInfesterState_l5cfa94 ball;  // 0x146d0
};
static_assert(__builtin_offsetof(UnkInfesterBallAI_l5cfa94, ball) == 0x146d0, "layout");

// Sets the burst id on the owner's timeline to the all-flags mask.
inline void SetBurstTrack_l5cfa94(UnkTimelineComponent* timeline) {
    u32 mask = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
    fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a120), &mask, false, -1.0f);
}

struct UnkInfesterBallAI_b60;

void fn_71005cfa94(UnkInfesterBallAI_b60* ai, bool flag) {
    UnkInfesterBallAI_l5cfa94* self = reinterpret_cast<UnkInfesterBallAI_l5cfa94*>(ai);
    UnkInfesterState_l5cfa94& ball = self->ball;
    if (ball.burst)
        return;
    if (!flag)
        ball.field_0x3e = true;
    if (ball.state != 2)
        ball.state = 2;

    UnkParams5_l5cfa94 params;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x34 = 5;
    params.field_0x30 = 0;
    params.field_0x39 = 0;
    params.field_0x3d = 0;
    params.field_0x3a = 1;
    params.field_0x3b = 1;
    fn_71006f4b0c(self->owner->field_0x298, &params, 0);
    ball.burst = true;

    UnkInfesterShockWave_l5cfa94 wave;
    AssignRef(&wave.source, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner)));
    wave.Name() = g_unk_71726ecb68;
    UnkActor* owner = self->owner;
    wave.position.x = owner->field_0x44;
    wave.position.y = owner->field_0x54;
    wave.field_0x120 = 0;
    wave.position.z = owner->field_0x64;
    UnkInfesterTuning_l5cfa94* tuning = ball.tuning;
    wave.field_0x124 = tuning->field_0x510;
    wave.field_0x130 = tuning->field_0x514;
    wave.field_0x10c = 1;
    wave.field_0x134 = tuning->field_0x518;
    wave.Id() = g_unk_71726ecb60;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(wave.request),
                  reinterpret_cast<const UnkInfesterTunables_l5cfa94*>(&g_unk_7172716160)->field_0x43c);
    wave.field_0x190 = 0;
    fn_71009f51f8(reinterpret_cast<char*>(&wave), self->owner);

    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8))
        SetBurstTrack_l5cfa94(timeline);
    fn_7100d7b054(self->owner, 0.1f);
}
