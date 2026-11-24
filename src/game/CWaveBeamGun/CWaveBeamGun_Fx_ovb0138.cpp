// CWaveBeamGun's unit (tools/tu_map.py): registering the wave beam's explosion effects and model with
// the effect cache of the lazily created model manager, once (flag at +0x1870), before the base step
// (fn_7100867b2c, the shape of CSPBGun's fn_710085e1c8 in src/game/CSPBGun/CSPBGun_b51.cpp), and
// releasing them again before the other base step (fn_7100867d14). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/CPlasmaBeamGun.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU2739.h"

using base::global::CFilePathStrId;

struct UnkFxCache;

struct UnkWaveBeamGun_ovb0138 {
    u8 pad_0x0[0x1870];
    bool fx_registered;
};

struct UnkPlasmaBeamGun_ovb0135;
void CPowerBeamGun_PreloadEffects(UnkPlasmaBeamGun_ovb0135*);
void CPowerBeamGun_ReleaseEffects(UnkPlasmaBeamGun_ovb0135*);

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

static inline void ReleaseFx(const char* path) {
    UnkFxCache* cache = GetCache();
    CFilePathStrId id(path, true);
    fn_71011aa5a4(cache, &id);
}

void fn_7100867b2c(UnkWaveBeamGun_ovb0138* self) {
    if (!self->fx_registered) {
        PreloadFx("actors/weapons/wavebeam/fx/explosion.bcptl", 0x1f);
        PreloadFx("actors/weapons/wavebeam/fx/minor_explosion.bcptl", 0xc);
        PreloadModel("actors/weapons/wavebeam/fx/explosion_game.bcmdl", 0xc);
        self->fx_registered = true;
    }
    CPowerBeamGun_PreloadEffects(reinterpret_cast<UnkPlasmaBeamGun_ovb0135*>(self));
}

void fn_7100867d14(UnkWaveBeamGun_ovb0138* self) {
    if (self->fx_registered) {
        ReleaseFx("actors/weapons/wavebeam/fx/explosion.bcptl");
        ReleaseFx("actors/weapons/wavebeam/fx/minor_explosion.bcptl");
        ReleaseFx("actors/weapons/wavebeam/fx/explosion_game.bcmdl");
        self->fx_registered = false;
    }
    CPowerBeamGun_ReleaseEffects(reinterpret_cast<UnkPlasmaBeamGun_ovb0135*>(self));
}
