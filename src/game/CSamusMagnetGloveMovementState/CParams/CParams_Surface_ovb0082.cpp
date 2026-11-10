// In the CSamusMagnetGloveMovementState::CParams unit: while the state holds a surface whose owner has an
// animation, fn_71009ab0cc restarts the loop rumble (fn_71009aefb8) when its current track is a
// CCharClassActiveRumbleOnMagnetSurfaceTrack; otherwise it fades out (fn_7100072328, 0.2 and 0) the rumble the
// handle at +0x338 still refers to. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkHandleTarget_ovb0082 {
    void* target;
    u64 serial;
};

struct UnkHandle {
    UnkHandleTarget_ovb0082* target;
    u64 serial;

    void* Get() const {
        if (target && target->serial == serial && serial != ~0ull)
            return target->target;
        return nullptr;
    }
};

extern UnkHandle g_unk_7101d586c0;
void fn_7100072328(void*, f32, f32);

struct UnkMagnetSurfaceOwner_ovb0082 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkMagnetSurfaceRef_ovb0082 {
    u8 pad_0x0[0x10];
    UnkMagnetSurfaceOwner_ovb0082* owner;
};

struct UnkMagnetRumbleState_ovb0082 {
    u8 pad_0x0[0x1e8];
    UnkMagnetSurfaceRef_ovb0082* surface;
    u8 pad_0x1f0[0x148];
    UnkHandle rumble;  // 0x338
};
static_assert(__builtin_offsetof(UnkMagnetRumbleState_ovb0082, rumble) == 0x338, "layout");

void fn_71009aefb8(UnkMagnetRumbleState_ovb0082* self);

void fn_71009ab0cc(UnkMagnetRumbleState_ovb0082* self) {
    if (!self->surface || !self->surface->owner)
        return;
    UnkAnimationComponent* animation = self->surface->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100befa38())) {
        fn_71009aefb8(self);
        return;
    }
    if (self->rumble.Get() != g_unk_7101d586c0.Get())
        fn_7100072328(self->rumble.Get(), 0.2f, 0.0f);
}
