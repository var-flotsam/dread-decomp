// CSPBGun (unit 1317): after the base step, registers the power beam's two explosion effects with
// the effect cache of the lazily created model manager, once (flag at +0x1870). Names and layouts
// are placeholders. fn_710085e328 unregisters them again.
#include "base/global/CFilePathStrId.h"
#include "base/UnkVector.h"
#include "game/CPlasmaBeamGun.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU2739.h"

using base::global::CFilePathStrId;

struct UnkFxCache;

struct UnkSPBGun {
    u8 pad_0x0[0x1870];
    bool fx_registered;
};

void fn_710083593c(UnkSPBGun* self);

static inline UnkModelManager* GetManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

static inline void PreloadFx(const char* path, u32 kind) {
    UnkFxCache* cache = reinterpret_cast<UnkFxCache*>(GetManager());
    CFilePathStrId id(path, true);
    fn_7100851bc0(cache, &id, kind, false);
}

void fn_710085e1c8(UnkSPBGun* self) {
    fn_710083593c(self);
    if (self->fx_registered)
        return;
    PreloadFx("actors/weapons/powerbeam/fx/explosion.bcptl", 0xf);
    PreloadFx("actors/weapons/powerbeam/fx/explosioncharge.bcptl", 3);
    self->fx_registered = true;
}

void fn_7100835940(UnkSPBGun* self);

static inline void ReleaseFx(const char* path) {
    UnkFxCache* cache = reinterpret_cast<UnkFxCache*>(GetManager());
    CFilePathStrId id(path, true);
    fn_71011aa5a4(cache, &id);
}

// The other way round: after the base step, unregisters the two effects again.
void fn_710085e328(UnkSPBGun* self) {
    fn_7100835940(self);
    if (!self->fx_registered)
        return;
    ReleaseFx("actors/weapons/powerbeam/fx/explosion.bcptl");
    ReleaseFx("actors/weapons/powerbeam/fx/explosioncharge.bcptl");
    self->fx_registered = false;
}
