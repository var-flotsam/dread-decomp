// CScorpiusFXComponent: places each effect object along a line at multiples of a step distance,
// switching those beyond both limits off. Names and layouts are placeholders.

#include "game/UnkSwitchObject.h"
#include "types.h"

void fn_7100146dac(UnkSwitchObject* self, bool enable);
void fn_7100146d84(UnkSwitchObject* self, f32* position);

struct UnkScorpiusFX {
    u8 pad_0x0[0x48];
    f32 origin[3];
    f32 dir[3];
    f32 step;
    u32 count;
    UnkSwitchObject* objects[1];
};

void fn_710080f3c4(UnkScorpiusFX* self, f32 limitA, f32 limitB) {
    if (self->step <= 0.01f)
        return;
    for (u64 i = 0; i < self->count; i++) {
        UnkSwitchObject* obj = self->objects[i];
        f32 t = self->step * (f32)(u32)i;
        if (t < limitA || t > limitB) {
            fn_7100146e60(obj, false);
        } else {
            alignas(16) f32 pos[3];
            f32 x = t * self->dir[0];
            x += self->origin[0];
            f32 y = t * self->dir[1];
            y += self->origin[1];
            f32 z = t * self->dir[2];
            z += self->origin[2];
            pos[0] = x;
            pos[1] = y;
            pos[2] = z;
            fn_7100146dac(obj, true);
            fn_7100146d84(obj, pos);
            fn_7100146e60(obj, true);
        }
    }
}
