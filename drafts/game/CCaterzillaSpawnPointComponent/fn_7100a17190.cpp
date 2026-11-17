// CCaterzillaSpawnPointComponent unit (tools/tu_map.py unit 1527): finds the link whose actor lies
// within 70 units of `pos` and faces a usable way out (its axis, column 2 of its transform, or column 1
// for the "spawnpointslidle..." actors), storing the distance and returning the index (-1 for none).
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkCaterzillaActor_ovb0054 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkCaterzillaTargetRef_ovb0054 {
    u8 pad_0x0[0x10];
    UnkCaterzillaActor_ovb0054* actor;  // 0x10
};

struct UnkCaterzillaLink_ovb0054 {
    u8 pad_0x0[0x8];
    u8 property[0x28];  // 0x8, a reflected property naming the actor
    u32 state;          // 0x30
    u8 pad_0x34[0x4];
    UnkCaterzillaTargetRef_ovb0054* target;  // 0x38, a base::UnkRefHandle

    UnkCaterzillaTargetRef_ovb0054* Target() {
        if (state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(property);
                fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&target),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return target;
    }
};

struct UnkNameHolder_b0161 {
    CStrId id;
};

// The actor seen as its transform (at +0x38; its translation is the x, y, z above).
struct UnkCaterzillaBody_la17190 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkCaterzillaSpawnPoint_la17190 {
    u8 pad_0x0[0x438];
    u8 links[0x8];   // 0x438
    u32 link_count;  // 0x440
};

static inline UnkCaterzillaActor_ovb0054* ActorOf(UnkCaterzillaTargetRef_ovb0054* target) {
    return target ? target->actor : nullptr;
}

static inline const UnkMatrix44& TransformOf(UnkCaterzillaActor_ovb0054* actor) {
    return fn_710002614c(&reinterpret_cast<UnkCaterzillaBody_la17190*>(actor)->transform);
}

// A point or direction of the level, flattened to the 2D plane.
static inline void Flatten(CVector2D* out, f32 x, f32 y, f32 z) {
    CVector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    fn_710007b430(out, &v);
}

static inline CVector2D Flatten(f32 x, f32 y, f32 z) {
    CVector2D out;
    Flatten(&out, x, y, z);
    return out;
}

static inline f32 Distance(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

// Whether the id names `name`, counting the read.
static inline bool IsNamed(const CStrId& id, const CRntString& name) {
    ++id.data->reads;
    return id.data->Key() == name.key;
}

static inline bool Near(f32 value, f32 target) {
    return value - 0.001f <= target && value + 0.001f >= target;
}

s32 fn_7100a17190(UnkCaterzillaSpawnPoint_la17190* self, const CVector2D* pos, const CVector2D* forward,
                  const CVector2D* side, f32* out_distance) {
    auto* links = reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(self->links);
    u32 count = self->link_count;
    for (u32 i = 0; i != count; ++i) {
        UnkCaterzillaActor_ovb0054* actor = fn_7100247340(links, i)->Target()->actor;
        f32 distance = Distance(*pos, Flatten(actor->x, actor->y, actor->z));
        if (distance < 70.0f) {
            const UnkMatrix44& m = TransformOf(ActorOf(fn_7100247340(links, i)->Target()));
            CVector2D dir;
            Flatten(&dir, m.m[2], m.m[6], m.m[10]);
            CStrId name = fn_7100232fcc(ActorOf(fn_7100247340(links, i)->Target()))->id;
            if (IsNamed(name, CRntString("spawnpointslidleout")) ||
                IsNamed(name, CRntString("spawnpointslidlemetallicout"))) {
                const UnkMatrix44& n = TransformOf(ActorOf(fn_7100247340(links, i)->Target()));
                dir = Flatten(n.m[1], n.m[5], n.m[9]);
            }
            if (!Near(dir.x * side->x + dir.y * side->y, -1.0f)) {
                f32 d = dir.x * forward->x + dir.y * forward->y;
                if (d < 0.0f || Near(d, 0.0f)) {
                    *out_distance = distance;
                    return i;
                }
            }
        }
    }
    return -1;
}
