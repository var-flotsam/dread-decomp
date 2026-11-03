// In the CKraidSpikeMovablePlatformComponent unit: fn_7100608994 writes the platform's displacement
// for a step of `dt` (the speed at +0x258 along -x) unless the game is paused (fn_7100deabe4). Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkKraidSpikeDelta {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkKraidSpikePlatform {
    u8 pad_0x0[0x258];
    f32 speed;
};

void fn_7100608994(UnkKraidSpikePlatform* self, void*, void*, f32 dt, UnkKraidSpikeDelta* out) {
    if (!fn_7100deabe4(g_unk_7101d47310)) {
        f32 x = -(self->speed * dt);
        f32 zero = dt * 0.0f;
        out->x = x;
        out->y = zero;
        out->z = zero;
    }
}
