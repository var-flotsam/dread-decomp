// CAbilityMultiLockon's unit (tools/tu_map.py, unit 431): has the actordef factory load the five
// lock-on missile marker definitions and, for each that is an entity, deactivates it and keeps its
// weak handle in its aim slot. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU341.h"
#include "game/UnkTU344.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

// The definitions' type. The unit's other statics live in its initialiser (fn_710031d5f0); this one is
// defined here so that it shares one base address with the marker names, as in the original.
static CStrId s_markerType asm("g_unk_71726af980")("lockonmissilemarker", true);

// The actordef type id a definition is loaded as.
struct alignas(8) UnkMarkerDefType_l31833c {
    u32 id;
    UnkMarkerDefType_l31833c(u32 type) : id(type) {}
    // The factory's view of it.
    operator const UnkPowerUpLifeType_b52&() const { return *reinterpret_cast<const UnkPowerUpLifeType_b52*>(this); }
};

// Has the actordef factory load the marker definition `path` names (actordef type 0xe).
static inline UnkReflObject* LoadMarker(const CRntString& path) {
    return reinterpret_cast<UnkReflObject*>(fn_710119438c(
        g_unk_71726a0220, reinterpret_cast<const char*>(&path), &s_markerType,
        UnkMarkerDefType_l31833c(0xe), nullptr));
}

struct UnkMultiLockon_l31833c {
    u8 pad_0x0[0x3a0];
    UnkWeakHandle* slots[5];  // 0x3a0
};

void fn_710031833c(UnkMultiLockon_l31833c* self) {
    static const CRntString s_markers[5] asm("g_unk_71726af990") = {
        "lockonmissilemarker01", "lockonmissilemarker02", "lockonmissilemarker03",
        "lockonmissilemarker04", "lockonmissilemarker05",
    };
    for (u32 i = 0; i < 5; ++i) {
        UnkReflObject* marker = LoadMarker(s_markers[i]);
        if (!marker || !fn_710009d50c(marker->vf_0x0(), fn_7100d70c20()))
            continue;
        fn_7100232864(reinterpret_cast<UnkEntityState*>(marker), false);
        AssignRef(&self->slots[i], fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(marker)));
    }
}
