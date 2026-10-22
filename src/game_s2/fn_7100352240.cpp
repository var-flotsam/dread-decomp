// Moves an analog-aim value toward the actor's current target: finds the aim-delta track on the
// actor's target, takes how far along it the actor is (or, without one, the gap between two
// stored values), and applies the change to the output signed by the track's direction.
#include "types.h"

struct UnkTrackQuery {
    void* field_0x0;
    struct UnkRange* field_0x8;
    f32 field_0x10;
};

struct UnkRange {
    u8 pad_0x0[0x10];
    f32 field_0x10;
    f32 field_0x14;
};

struct UnkAimTarget {
    u8 pad_0x0[1];
};

struct UnkAimActor {
    u8 pad_0x0[0x38];
    u8 field_0x38[8];
    u8 pad_0x40[0x1e0];
    UnkAimTarget* field_0x220;
};

struct UnkDirection {
    u8 pad_0x0[8];
    f32 field_0x8;
};

struct UnkAimComponent {
    u8 pad_0x0[0x30];
    UnkAimActor* field_0x30;
    u8 pad_0x38[0x638];
    f32 field_0x670;
    f32 field_0x674;
};

struct UnkAimOutput {
    u8 pad_0x0[0x60];
    f32 field_0x60;
    u8 pad_0x64[8];
    f32 field_0x6c;
};

void* fn_7100c50338();  // CCharClassSPAnalogAimKinematicDeltaTrack::refl_GetClass
void* fn_7100ceadf8();  // CCharClassAnalogAimTrack::refl_GetClass
void* fn_7100d2b3a8();  // CCharClassTurnTrack::refl_GetClass
void fn_710117b08c(UnkAimTarget* target, UnkTrackQuery* query);
bool fn_710117b348(UnkAimTarget* target, void* cls);
f32 fn_7101177ea0(UnkAimTarget* target, u64 key);
UnkDirection* fn_710002614c(void* actor_part);

void fn_7100352240(UnkAimComponent* self, UnkAimOutput* out) {
    if (!self->field_0x30->field_0x220)
        return;
    UnkTrackQuery query = {fn_7100c50338(), nullptr, -1.0f};
    fn_710117b08c(self->field_0x30->field_0x220, &query);
    f32 delta;
    if (query.field_0x8) {
        f32 value = fn_7101177ea0(self->field_0x30->field_0x220, 0xf4061ff25a2163d6) - query.field_0x8->field_0x10;
        if (!(value > 0.0f))
            return;
        f32 hi = query.field_0x8->field_0x14;
        f32 total = self->field_0x670;
        delta = value * (total / (hi - query.field_0x8->field_0x10)) - self->field_0x674;
        if (delta + 0.001f >= 0.0f && delta - 0.001f <= 0.0f)
            return;
    } else {
        if (!(self->field_0x674 < self->field_0x670))
            return;
        if (fn_710117b348(self->field_0x30->field_0x220, fn_7100ceadf8()) &&
            fn_710117b348(self->field_0x30->field_0x220, fn_7100d2b3a8()))
            return;
        delta = self->field_0x670 - self->field_0x674;
    }
    f32 direction = fn_710002614c(self->field_0x30->field_0x38)->field_0x8;
    f32 sign = (direction > 0.0f) - (direction < 0.0f);
    out->field_0x60 = delta * sign;
    out->field_0x6c = delta * sign;
    self->field_0x674 = delta + self->field_0x674;
}
