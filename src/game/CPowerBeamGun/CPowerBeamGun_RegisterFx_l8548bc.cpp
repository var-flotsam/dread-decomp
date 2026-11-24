// CPowerBeamGun's unit: after the base step, registers the power beam's effects and its waves model
// with the effect cache of the lazily created model manager, once (flag at +0x1870). Names and
// layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/CGun.h"
#include "game/CPlasmaBeamGun.h"
#include "game/UnkModelManager.h"
#include "game/UnkPlasmaBeamGun_ovb0135.h"

using base::global::CFilePathStrId;

struct UnkFxCache;

struct UnkSPBGun;

static inline UnkFxCache* GetCache() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return reinterpret_cast<UnkFxCache*>(g_unk_71726b0cd8);
}

static inline void PreloadFx(const char* path, u32 kind) {
    UnkFxCache* cache = GetCache();
    CFilePathStrId id(path, true);
    CPlasmaBeamGun_PreloadParticleFxPool(cache, &id, kind, false);
}

static inline void PreloadModel(const char* path, u32 kind) {
    UnkFxCache* cache = GetCache();
    CFilePathStrId id(path, true);
    CPlasmaBeamGun_PreloadModelFxPool(cache, &id, kind, false);
}

void CPowerBeamGun_PreloadEffects(UnkPlasmaBeamGun_ovb0135* self) {
    fn_710083593c(reinterpret_cast<UnkSPBGun*>(self));
    if (!self->fx_registered) {
        PreloadFx("actors/weapons/powerbeam/fx/explosion.bcptl", 0xf);
        PreloadFx("actors/weapons/powerbeam/fx/explosioncharge.bcptl", 3);
        PreloadFx("actors/weapons/powerbeam/fx/diffusion_beam.bcptl", 3);
        PreloadFx("actors/weapons/powerbeam/fx/minor_diffusion_beam.bcptl", 0xc);
        PreloadModel("actors/weapons/powerbeam/fx/diffusion_explosion_game.bcmdl", 0xf);
        PreloadFx("actors/weapons/powerbeam/fx/chargebeam.bcptl", 1);
        PreloadFx("actors/weapons/powerbeam/fx/chargeloop.bcptl", 1);
        PreloadFx("actors/weapons/powerbeam/fx/burstcharge.bcptl", 1);
        PreloadModel("actors/weapons/powerbeam/models/powerbeam_waves.bcmdl", 1);
        self->fx_registered = true;
    }
}
