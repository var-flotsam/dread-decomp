// CCharClassScourgeAIComponent's unit (tools/tu_map.py unit 1033): a state code: 4 when the actor's
// collision maps the object at +0x60 of the fourth argument to the string id g_unk_7172704c38, 1 otherwise
// (the empty id stands in when there is no object or no collision). Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CCollisionComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkCollisionContainer;

__attribute__((visibility("hidden"))) extern CStrId g_unk_7172704c38;

struct UnkScourgeActor_l69e8cc {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkScourgeHit_l69e8cc {
    u8 pad_0x0[0x60];
    void* object;  // 0x60
};

// The id the actor's collision gives the object; the empty id without either.
static inline const CStrId& CollisionId(UnkScourgeActor_l69e8cc* actor, void* object) {
    if (object && actor->collision)
        return *fn_7100864018(actor->collision, object);
    return g_unk_7101d470e0;
}

u32 fn_710069e8cc(void* self, UnkScourgeActor_l69e8cc* actor, void* arg, UnkScourgeHit_l69e8cc* hit) {
    const CStrId& id = hit ? CollisionId(actor, hit->object) : g_unk_7101d470e0;
    return id.data == g_unk_7172704c38.data ? 4 : 1;
}
