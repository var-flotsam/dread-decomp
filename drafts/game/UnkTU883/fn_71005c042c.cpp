// CHydrogigaAIComponent's unit (tools/tu_map.py): the box around the owner's position (fn_7100d7c9f4) as
// two corners written to `out` (min, then max): fixed offsets (guarded statics, each carrying the third
// component of the player's position) added to the position with its y raised by the owner's +0x394.
// Does nothing without a player or an owner (+0x30). Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPlayer;
struct UnkDropCameraVec3;
UnkPlayer* fn_7100dec788(UnkGameManager*);
const UnkDropCameraVec3* fn_7100d7c9f4(UnkPlayer* player);

struct UnkHydrogigaAI_b0166 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

struct UnkBoxCorners_b0166 {
    UnkVec3f min;
    UnkVec3f max;
};

static inline UnkVec3f Add(const UnkVec3f& a, const UnkVec3f& b) { return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z); }

void fn_71005c042c(UnkHydrogigaAI_b0166* self, UnkBoxCorners_b0166* out) {
    __builtin_memset(out, 0, sizeof(*out));
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    UnkActor* owner = self->owner;
    if ((player == nullptr) | (owner == nullptr))
        return;
    f32 depth = reinterpret_cast<const UnkVec3f*>(fn_7100d7c9f4(player))->z;
    const UnkVec3f* p = reinterpret_cast<const UnkVec3f*>(fn_7100d7c9f4(reinterpret_cast<UnkPlayer*>(self->owner)));
    UnkVec3f pos(p->x, p->y + *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(self->owner) + 0x394), p->z);
    static const UnkVec3f s_max(200.0f, 60.0f, depth);
    static const UnkVec3f s_min(-200.0f, -40.0f, depth);
    out->min = Add(pos, s_min);
    out->max = Add(pos, s_max);
}
