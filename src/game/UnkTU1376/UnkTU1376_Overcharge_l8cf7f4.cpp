// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): turns the door's overcharge on or
// off (flag at +0x418), preloading its two effects in the lazily created model manager's effect cache or
// releasing them again. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/CPlasmaBeamGun.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU2739.h"

using base::global::CFilePathStrId;

struct UnkFxCache;

struct UnkDoorLife_l8cf7f4 {
    u8 pad_0x0[0x418];
    bool overcharged;
};

static inline UnkFxCache* GetCache() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return reinterpret_cast<UnkFxCache*>(g_unk_71726b0cd8);
}

static inline void PreloadFx(UnkFxCache* cache, const char* path) {
    CFilePathStrId id(path, true);
    fn_7100851bc0(cache, &id, 1, false);
}

static inline void ReleaseFx(UnkFxCache* cache, const char* path) {
    CFilePathStrId id(path, true);
    fn_71011aa5a4(cache, &id);
}

void fn_71008cf7f4(UnkDoorLife_l8cf7f4* self, bool on) {
    if (self->overcharged == on)
        return;
    self->overcharged = on;
    UnkFxCache* cache = GetCache();
    if (on) {
        PreloadFx(cache, "actors/props/door/fx/door_open_overcharge.bcptl");
        PreloadFx(GetCache(), "actors/props/door/fx/door_open_overcharge_activ.bcptl");
    } else {
        ReleaseFx(cache, "actors/props/door/fx/door_open_overcharge.bcptl");
        ReleaseFx(GetCache(), "actors/props/door/fx/door_open_overcharge_activ.bcptl");
    }
}
