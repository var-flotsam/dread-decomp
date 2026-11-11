// CSynchronizedBlockCameraCtrl unit (tools/tu_map.py, the name from reflection): the constructor
// (fn_7100a20b10: the base camera constructor fn_7100763ed4, then vtable g_unk_7101a8d240 and the members);
// the start (fn_7100a20e00: as CHydrogigaGrabCameraCtrl's fn_710086ea48, then the player's x and angles
// kept at +0x314); the end (fn_7100a20f64: as fn_710086eb44, then the spans at +0x2ec cleared).
// Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCameraBoundaryCtrl.h"
#include "game/CCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkStruct_7100763ed4;

extern void* g_unk_7101a8d240[];  // vtable

// A string id that starts null and takes the pool's empty string (the pool created first when missing).
struct UnkSyncEmptyId_b0129 {
    base::global::UnkStringData* data;

    UnkSyncEmptyId_b0129() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkSyncSpan_b0129 {
    f32 values[7];
};

struct UnkSyncBlockCameraCtrlCtor_b0129 {
    void* vtable;
    u8 unk_0x8[0x20];
    u32 field_0x28;  // 0x28
    u8 unk_0x2c[0x35 - 0x2c];
    bool field_0x35;
    u8 unk_0x36[0x9c - 0x36];
    u32 field_0x9c;
    u8 unk_0xa0[4];
    u32 field_0xa4;
    u8 unk_0xa8[0xc0 - 0xa8];
    f32 field_0xc0;
    f32 field_0xc4;
    u8 unk_0xc8[0x2a0 - 0xc8];
    u64 field_0x2a0;
    u64 field_0x2a8;
    u64 field_0x2b0;
    u64 field_0x2b8;
    CStrId field_0x2c0;
    u64 field_0x2c8;
    u32 field_0x2d0;
    u8 unk_0x2d4[0xc];
    UnkSyncEmptyId_b0129 field_0x2e0;
    bool field_0x2e8;
    u8 unk_0x2e9[3];
    UnkSyncSpan_b0129 field_0x2ec;
    f32 field_0x308;
    f32 field_0x30c;
    f32 field_0x310;
    UnkSyncSpan_b0129 field_0x314;
};
static_assert(__builtin_offsetof(UnkSyncBlockCameraCtrlCtor_b0129, field_0x314) == 0x314, "layout");

void fn_7100a20b10(UnkSyncBlockCameraCtrlCtor_b0129* self) {
    fn_7100763ed4(reinterpret_cast<UnkStruct_7100763ed4*>(self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a8d240) + 0x10;
    self->field_0x2a0 = 0;
    self->field_0x2a8 = 0;
    self->field_0x2b0 = 0;
    self->field_0x2b8 = 0;
    new (&self->field_0x2c0) CStrId(g_unk_710162e2ed, true);
    self->field_0x2c8 = 0;
    self->field_0x2d0 = 0;
    new (&self->field_0x2e0) UnkSyncEmptyId_b0129;
    self->field_0x2e8 = false;
    __builtin_memset(&self->field_0x2ec, 0, sizeof(UnkSyncSpan_b0129));
    self->field_0x308 = -1.0f;
    self->field_0x30c = 0.0f;
    self->field_0x310 = 0.96f;
    __builtin_memset(&self->field_0x314, 0, sizeof(UnkSyncSpan_b0129));
    self->field_0x28 = 3;
    self->field_0x35 = true;
    self->field_0xa4 = 0;
    self->field_0x9c = 0;
    self->field_0xc0 = 1000.0f;
    self->field_0xc4 = 1000.0f;
}

struct UnkFixedCamera_fc;
struct UnkArg_fc;
struct UnkVec;
struct UnkGrabCameraCtrl;
void fn_7100e038b0(UnkGameManager*, void*, UnkVec*, u64);
void fn_7100e03a78(UnkGameManager*, void*);
void fn_7100df6c78(UnkGameManager* self, f32 a, f32 b);
void fn_7100df6d3c(UnkGameManager* self, void* settings, f32 a, f32 b);

struct UnkSyncDef_b0129 {
    u8 pad_0x0[0xb4];
    UnkVec3f dir;
};

struct UnkSyncArg_b0129 {
    u8 pad_0x0[0x20];
    UnkXformNode node;  // 0x20
    UnkSyncDef_b0129* def;  // 0x130
};

struct UnkSyncSettings_b0129 {
    u8 pad_0x0[0x10];
    u8 body[0x34];
    union {
        s32 ref_count;  // 0x44
    };
};

struct UnkSyncPlayer2_b0129 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkSyncCamera_b0129 {
    u8 pad_0x0[0x28f];
    bool field_0x28f;
    u8 pad_0x290[0x18];
    UnkVec3f dir;  // 0x2a8
    UnkVec3f pos;  // 0x2b4
    CStrId camera;  // 0x2c0
    u8 field_0x2c8[0xc];
    f32 timer;  // 0x2d4
    f32 field_0x2d8;
    u8 pad_0x2dc[0x4];
    UnkSyncSettings_b0129* settings;  // 0x2e0
    bool finished;  // 0x2e8
    u8 pad_0x2e9[3];
    UnkVec3f field_0x2ec;
    UnkVec3f field_0x2f8;
    f32 field_0x304;
    f32 field_0x308;
    u32 field_0x30c;
    u8 pad_0x310[4];
    f32 player_x;  // 0x314
    UnkVec3f player_angle;  // 0x318
};
static_assert(__builtin_offsetof(UnkSyncCamera_b0129, player_angle) == 0x318, "layout");

void fn_7100a20e00(UnkSyncCamera_b0129* self, UnkSyncArg_b0129* arg) {
    self->finished = false;
    self->dir = arg->def->dir;
    self->pos = fn_7100206154(&arg->node);
    if (self->camera.data != &g_unk_7101d47108->empty)
        fn_7100e038b0(g_unk_7101d47310, &self->camera, reinterpret_cast<UnkVec*>(self->field_0x2c8), 0);
    if (self->timer > 0.0f) {
        fn_7100df6c78(g_unk_7101d47310, 1.0f, 0.0f);
        UnkGameManager* m = g_unk_7101d47310;
        f32 a = self->field_0x2d8, b = self->timer;
        ++self->settings->ref_count;
        fn_7100df6d3c(m, self->settings->body, a, b);
    }
    if (auto* player = reinterpret_cast<UnkSyncPlayer2_b0129*>(fn_7100dec788(g_unk_7101d47310))) {
        self->player_x = player->xform.m[0][3];
        self->player_angle = player->xform.GetAngle();
    }
    fn_71010ed98c(reinterpret_cast<UnkFixedCamera_fc*>(self), reinterpret_cast<UnkArg_fc*>(arg), 1);
}

void fn_7100a20f64(UnkSyncCamera_b0129* self) {
    fn_71010edd64(reinterpret_cast<UnkGrabCameraCtrl*>(self));
    if (self->camera.data != &g_unk_7101d47108->empty) {
        fn_7100e03a78(g_unk_7101d47310, &self->camera);
        self->camera = g_unk_7101d470e0;
    }
    if (self->timer > 0.0f) {
        fn_7100df6c78(g_unk_7101d47310, 1.0f, 0.0f);
        // The timer and the word after it, cleared as one doubleword.
        *reinterpret_cast<u64*>(reinterpret_cast<char*>(self) + 0x2d4) = 0;
        reinterpret_cast<CStrId&>(self->settings) = g_unk_7101d470e0;
    }
    self->field_0x2ec = UnkVec3f(0.0f, 0.0f, 0.0f);
    self->field_0x2f8 = UnkVec3f(0.0f, 0.0f, 0.0f);
    self->field_0x304 = -1.0f;
    self->field_0x308 = -1.0f;
    self->field_0x30c = 0;
    self->field_0x28f = false;
}
