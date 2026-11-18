// CAnimatedCameraCtrl's unit (tools/tu_map.py): the update. Once the animation is within the blend time
// (+0x2cc) of its end, the player's input is given back, the camera manager releases the id at +0x5a0
// (unless the game is paused or the player is grabbed) and the blend runs; then the base update.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CGrabComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2730.h"
#include "types.h"

using base::global::CStrId;

struct UnkManager;

// A bit mask passed by value (copied into a temporary), and its two guarded constants.
struct UnkCamMask_b0101 {
    u32 bits;
    UnkCamMask_b0101() : bits(0) {}
    UnkCamMask_b0101(u32 b) : bits(b) {}
    UnkCamMask_b0101(const UnkCamMask_b0101& o) : bits(o.bits) {}
};
template <class T>
struct UnkCamMaskOf_b0101 {
    static const UnkCamMask_b0101& None() {
        static UnkCamMask_b0101 s;
        return s;
    }
    static const UnkCamMask_b0101& All() {
        static UnkCamMask_b0101 s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkCamMaskTag_b0101;

static inline void* CameraManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return manager;
}

struct UnkPlayer;
struct UnkInputComponent;
extern __attribute__((visibility("hidden"))) char g_unk_7172a04768[];

struct CSynchronizedBlockCameraCtrl;
void fn_71007642d4(CSynchronizedBlockCameraCtrl* self, f32 t, bool flag);

struct UnkAnimatedCameraUpdate_l0ea53c;
void fn_71010ee8bc(UnkAnimatedCameraUpdate_l0ea53c* self, f32 t);

struct UnkPlayer_l0ea53c {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
    u8 pad_0x1e0[0x80];
    void* grab;  // 0x260
};

// The game state singleton as this function sees it.
struct UnkGameState_l0ea53c {
    u8 pad_0x0[0xb14];
    bool field_0xb14;
};

struct UnkCamAnimation_l0ea53c {
    u8 pad_0x0[0xdc];
    f32 end;  // 0xdc
};

struct UnkAnimatedCameraUpdate_l0ea53c {
    u8 pad_0x0[0x2a8];
    UnkCamAnimation_l0ea53c* animation;  // 0x2a8
    u8 pad_0x2b0[0x14];
    f32 field_0x2c4;
    u8 pad_0x2c8[0x4];
    f32 blend_time;  // 0x2cc
    u8 pad_0x2d0[0x1d0];
    f32 field_0x4a0;
    u8 pad_0x4a4[0xfc];
    CStrId field_0x5a0;
    u8 pad_0x5a8[0xb0];
    bool field_0x658;
};
static_assert(__builtin_offsetof(UnkAnimatedCameraUpdate_l0ea53c, field_0x658) == 0x658, "layout");

void fn_71010ea53c(UnkAnimatedCameraUpdate_l0ea53c* self, f32 dt, bool flag) {
    f32 end = self->animation ? self->animation->end : 0.0f;
    f32 blend = self->blend_time;
    if (blend > 0.0f) {
        // The stop time (+0x4a0) when it is set (not -1) and comes before the end.
        f32 stop = self->field_0x4a0;
        f32 limit = stop != -1.0f && stop < end ? stop : end;
        if (self->field_0x2c4 >= limit - blend) {
            auto* player = reinterpret_cast<UnkPlayer_l0ea53c*>(fn_7100dec788(g_unk_7101d47310));
            if (player && player->input && fn_710087238c(player->input))
                fn_7100872460(player->input, true, false, g_unk_7172a04768);
            if (self->field_0x5a0.data != &g_unk_7101d47108->empty && !fn_7101196fd8(g_unk_7101d47330) &&
                !static_cast<UnkGameState_l0ea53c*>(g_unk_7101d47330)->field_0xb14 &&
                !(player && player->grab && fn_710113d83c(player->grab))) {
                fn_7100eed3a4(CameraManager(), &self->field_0x5a0, UnkCamMaskOf_b0101<UnkCamMaskTag_b0101>::All(),
                              false);
                self->field_0x5a0 = g_unk_7101d470e0;
            }
            if (self->field_0x658)
                fn_71010ee8bc(self, flag ? -1.0f : dt);
        }
    }
    fn_71007642d4(reinterpret_cast<CSynchronizedBlockCameraCtrl*>(self), dt, flag);
}
