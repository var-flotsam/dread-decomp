// CExplosionGfx's unit (tools/tu_map.py): the largest of fLength and the two vRandomDelay bounds, never below
// zero.
#include "game/CExplosionGfx.h"
#include "game/CExplosionGfxLayout.h"
#include "types.h"

static inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }

f32 CExplosionGfx_GetMaxOfLengthAndRandomDelay(const CExplosionGfx* self) {
    f32 m = Max(self->fLength, Max(self->vRandomDelay[0], self->vRandomDelay[1]));
    return m < 0.0f ? 0.0f : m;
}
