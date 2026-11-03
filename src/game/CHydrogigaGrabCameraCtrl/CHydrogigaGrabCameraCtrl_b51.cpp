// CHydrogigaGrabCameraCtrl (unit 1324): after the base update, hands the game manager the pending
// camera id at +0x1d8 and clears it; while the timer at +0x1ec runs, resets the manager's blend
// (fn_7100df6c78) and clears the timer and the id at +0x1f8. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"

using base::global::CStrId;

struct UnkGrabCameraCtrl;
void fn_71010edd64(UnkGrabCameraCtrl* self);
void fn_7100df6c78(UnkGameManager* self, f32 a, f32 b);
void fn_7100e03a78(UnkGameManager*, void*);

struct UnkGrabCameraCtrl {
    u8 pad_0x0[0x1d8];
    CStrId camera;  // 0x1d8
    u8 pad_0x1e0[0xc];
    f32 timer;      // 0x1ec
    f32 field_0x1f0;
    u8 pad_0x1f4[0x4];
    CStrId field_0x1f8;
};

void fn_710086eb44(UnkGrabCameraCtrl* self) {
    fn_71010edd64(self);
    if (self->camera.data != &g_unk_7101d47108->empty) {
        fn_7100e03a78(g_unk_7101d47310, &self->camera);
        self->camera = g_unk_7101d470e0;
    }
    if (self->timer > 0.0f) {
        fn_7100df6c78(g_unk_7101d47310, 1.0f, 0.0f);
        // The timer and the word after it, cleared as one doubleword.
        *reinterpret_cast<u64*>(reinterpret_cast<char*>(self) + 0x1ec) = 0;
        self->field_0x1f8 = g_unk_7101d470e0;
    }
}
