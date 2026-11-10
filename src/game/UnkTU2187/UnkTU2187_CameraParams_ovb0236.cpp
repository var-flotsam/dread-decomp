// Unit 2187 (no reliable name): when the game manager has no camera controller to ask, or the current one is a
// CAnimatedCameraCtrl, hands two values of a pair (read from the owner's pointer, in swapped order) to the render manager
// under two pass-parameter hashes (fn_7100219294). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkFloatPair_b0236g {
    u32 first, second;
};

struct UnkOwner_b0236g {
    UnkFloatPair_b0236g* pair;
};

void fn_7100c7ba2c(UnkOwner_b0236g* self) {
    if (fn_7100deabe4(g_unk_7101d47310)) {
        UnkCameraCtrl* camera = fn_71010e2080(fn_7100defcec(g_unk_7101d47310));
        if (!camera || !fn_710009d50c(camera->vf_0x0(), fn_71010e7f90()))
            return;
    }
    UnkFloatPair_b0236g* pair = self->pair;
    u32 x = pair->first;
    u32 y = pair->second;
    fn_7100219294(g_unk_7101d47300, 0xd8cb39e0e66e0d64, reinterpret_cast<f32*>(&x), 0);
    fn_7100219294(g_unk_7101d47300, 0x9daa3a4588a370dc, reinterpret_cast<f32*>(&y), 0);
}
