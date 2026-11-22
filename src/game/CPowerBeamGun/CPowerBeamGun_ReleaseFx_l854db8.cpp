// CPowerBeamGun's unit: after the base step, releases the power beam's effects from the effect cache
// of the lazily created model manager, once registered (flag at +0x1870). Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/CGun.h"
#include "game/UnkModelManager.h"
#include "game/UnkPlasmaBeamGun_ovb0135.h"
#include "game/UnkTU2739.h"

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

static inline void ReleaseFx(const char* path) {
    UnkFxCache* cache = GetCache();
    CFilePathStrId id(path, true);
    fn_71011aa5a4(cache, &id);
}

void fn_7100854db8(UnkPlasmaBeamGun_ovb0135* self) {
    fn_7100835940(reinterpret_cast<UnkSPBGun*>(self));
    if (self->fx_registered) {
        ReleaseFx("actors/weapons/powerbeam/fx/explosion.bcptl");
        ReleaseFx("actors/weapons/powerbeam/fx/explosioncharge.bcptl");
        ReleaseFx("actors/weapons/powerbeam/fx/diffusion_beam.bcptl");
        ReleaseFx("actors/weapons/powerbeam/fx/minor_diffusion_beam.bcptl");
        ReleaseFx("actors/weapons/powerbeam/fx/diffusion_explosion_game.bcmdl");
        self->fx_registered = false;
    }
}
