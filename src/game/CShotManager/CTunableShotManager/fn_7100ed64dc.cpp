// CShotManager::CTunableShotManager unit: takes the first free effect slot, starts it for a type
// and variant at the given place and queues it on that type's explosion. Names and layouts are
// placeholders.
#include "game/CExplosionGfx.h"
#include "game/CShotManager/UnkShotEffect_b0011.h"
#include "types.h"

struct UnkShotManager_ed64dc {
    u8 pad_0x0[0x30];
    UnkShotEffect_b0011* effects;
    u32 effectCount;
    u8 pad_0x3c[0x240 - 0x3c];
    CExplosionGfx* field_0x240[5][5];
};

void fn_7100ddec68(UnkShotEffect_b0011* self, const void* a, const void* b);

void CShotManager_StartExplosionEffect(UnkShotManager_ed64dc* self, const void* a, const void* b, u32, u32 type, u32, u32 variant) {
    for (UnkShotEffect_b0011 *it = self->effects, *end = self->effects + self->effectCount; it != end; ++it) {
        if (it->state.live)
            continue;
        CExplosionGfx* explosion = self->field_0x240[type][variant];
        fn_7100ddec50(it, 1, 1);
        fn_7100ddec68(it, a, b);
        it->state.variant = variant;
        it->state.type = type;
        it->state.field_0x20 = fn_7100de1388(explosion);
        it->state.field_0x24 = 0;
        // The effect slot is the explosion's target (its position at +0xc).
        fn_7100de08f0(explosion, reinterpret_cast<UnkExplosionTarget*>(it));
        return;
    }
}
