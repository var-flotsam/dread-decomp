// CSamusCrouchMovementState::CParams unit: moves the crouch target toward its stored offset, scaled by
// how far the actor is along its crouch-reposition track (or fully when there is none).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkRepositionActor {
    u8 pad_0x0[0x38];
    UnkMatrix44 field_0x38;
    u8 pad_0x78[0x1a8];
    UnkAnimationComponent* field_0x220;
};

struct UnkRepositionState {
    u8 pad_0x0[0x30];
    UnkRepositionActor* field_0x30;
    u8 pad_0x38[0x164];
    f32 field_0x19c;
    f32 field_0x1a0;
    f32 field_0x1a4;
    f32 field_0x1a8;
};

struct UnkRepositionOutput {
    u8 pad_0x0[0x10];
    f32 field_0x10;
    u8 pad_0x14[0xc];
    f32 field_0x20;
    u8 pad_0x24[0xc];
    f32 field_0x30;
    u8 pad_0x34[0x2c];
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    f32 field_0x70;
    f32 field_0x74;
};
static_assert(__builtin_offsetof(UnkRepositionActor, field_0x220) == 0x220, "");
static_assert(__builtin_offsetof(UnkRepositionState, field_0x19c) == 0x19c, "");
static_assert(__builtin_offsetof(UnkRepositionOutput, field_0x60) == 0x60, "");

inline f32 Clamp(f32 value, f32 lo, f32 hi) {
    f32 upper = value >= hi ? hi : value;
    return value > lo ? upper : lo;
}

void fn_71009904c8(UnkRepositionState* self, UnkRepositionOutput* out) {
    if (self->field_0x1a8 == 0.0f)
        return;
    UnkAnimationComponent* animation = self->field_0x30->field_0x220;
    if (animation && fn_710117b348(animation, fn_7100c462c4())) {
        UnkTrackQuery query = {fn_7100c462c4(), nullptr, -1.0f};
        fn_710117b08c(self->field_0x30->field_0x220, &query);
        f32 length = query.result->field_0x14 - query.result->field_0x10;
        f32 progress = fn_7101177ea0(self->field_0x30->field_0x220, 0xf4061ff25a2163d6) - query.result->field_0x10;
        f32 ratio = Clamp(progress, 0.0f, length) / length;
        const UnkMatrix44& m = fn_710002614c(&self->field_0x30->field_0x38);
        f32 dirX = m.m[2];
        f32 dirY = m.m[6];
        f32 dirZ = m.m[10];
        f32 scale = self->field_0x1a8;
        f32 x = self->field_0x19c + ratio * (dirX * scale) - out->field_0x10;
        f32 y = self->field_0x1a0 + ratio * (dirY * scale) - out->field_0x20;
        f32 z = self->field_0x1a4 + ratio * (dirZ * scale) - out->field_0x30;
        out->field_0x60 = x;
        out->field_0x64 = y;
        out->field_0x68 = z;
        out->field_0x6c = x;
        out->field_0x70 = y;
        out->field_0x74 = z;
        return;
    }
    const UnkMatrix44& m = fn_710002614c(&self->field_0x30->field_0x38);
    f32 dirX = m.m[2];
    f32 dirY = m.m[6];
    f32 dirZ = m.m[10];
    f32 scale = self->field_0x1a8;
    f32 x = self->field_0x19c + dirX * scale - out->field_0x10;
    f32 y = self->field_0x1a0 + dirY * scale - out->field_0x20;
    f32 z = self->field_0x1a4 + dirZ * scale - out->field_0x30;
    out->field_0x60 = x;
    out->field_0x64 = y;
    out->field_0x68 = z;
    out->field_0x6c = x;
    out->field_0x70 = y;
    out->field_0x74 = z;
    self->field_0x1a8 = 0.0f;
}
