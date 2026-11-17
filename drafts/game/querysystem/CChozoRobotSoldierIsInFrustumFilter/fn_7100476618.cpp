// querysystem::CChozoRobotSoldierIsInFrustumFilter's unit: rejects a position for a Chozo robot soldier
// when the box around it, the actor's target and the other soldiers no longer fits the camera's view.
// Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/tunable/CTunable.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU365.h"
#include "types.h"

struct UnkBreakableTarget {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkFrustumCamera_l476618 {
    u8 pad_0x0[0x6c];
    f32 fov;  // degrees
    u8 pad_0x70[0x4];
    f32 distance;
};

struct UnkSoldierAI_l476618 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x28];
    UnkActor* actor;
    u8 pad_0x38[0x146f0];
    u8 field_0x14728[0x8];
};
static_assert(__builtin_offsetof(UnkSoldierAI_l476618, field_0x14728) == 0x14728, "layout");

struct UnkSoldierEntry_l476618 {
    u8 pad_0x0[0x10];
    UnkSoldierAI_l476618* component;
};

struct UnkSoldierList_l476618 {
    UnkSoldierEntry_l476618** data;
    u32 count;

    UnkSoldierEntry_l476618** begin() const { return data; }
    UnkSoldierEntry_l476618** end() const { return data + count; }
};

struct UnkPosition_l476618 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
};

// What this function reads of the lazily created manager: the player's actor.
struct UnkManagerView_l476618 {
    u8 pad_0x0[0x2a0];
    UnkActor* player;
};

struct UnkFrustumFilter_l476618;

UnkFrustumCamera_l476618* fn_71003aff78(void* owner);
UnkSoldierList_l476618* fn_710046eee4();
UnkPosition_l476618* fn_71004739f8(UnkSoldierAI_l476618* soldier);

static inline UnkManager_71726af468* GetManager_l476618() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

inline bool IsPlayer_l476618(UnkActor* actor) {
    return reinterpret_cast<UnkManagerView_l476618*>(GetManager_l476618())->player == actor;
}

inline f32 Min_l476618(f32 a, f32 b) {
    return b < a ? b : a;
}

inline f32 Max_l476618(f32 a, f32 b) {
    return a < b ? b : a;
}

bool fn_7100476618(const UnkFrustumFilter_l476618* self, UnkActor* actor, const UnkActor* at) {
    if (!actor)
        return true;
    UnkSoldierAI_l476618* ai = static_cast<UnkSoldierAI_l476618*>(actor->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710046ea2c()))
        return true;

    f32 x = at->field_0x44;
    f32 y = at->field_0x54;
    if (actor->field_0x1f8)
        y = fn_7100977bec(static_cast<UnkFlagged*>(actor->field_0x1f8)) + y;

    UnkBreakableTarget* target = *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(ai->field_0x14728));
    if (!target || !target->field_0x10)
        return true;
    UnkFrustumCamera_l476618* camera = fn_71003aff78(target->field_0x10);
    f32 halfTan = tanf(static_cast<f32>(camera->fov * 3.14159265358979323846 / 180.0) * 0.5f);
    f32 width = (halfTan * camera->distance - 370.0f) * 2.0f;
    f32 aspect = fn_7100132fa0();

    UnkActor* focus = fn_7100d7c990(actor);
    f32 minX = Min_l476618(focus->field_0x44, x);
    f32 maxX = Max_l476618(focus->field_0x44, x);
    f32 minY = Min_l476618(focus->field_0x54, y);
    f32 maxY = Max_l476618(focus->field_0x54, y);

    UnkSoldierList_l476618* soldiers = fn_710046eee4();
    for (UnkSoldierEntry_l476618* entry : *soldiers) {
        UnkSoldierAI_l476618* other = entry ? entry->component : nullptr;
        if (other == ai)
            continue;
        f32 px, py;
        if (!IsPlayer_l476618(other->actor)) {
            UnkPosition_l476618* position = fn_71004739f8(other);
            if (!position)
                continue;
            px = position->x;
            py = position->y;
        } else if (IsPlayer_l476618(other->actor)) {
            // The player counts at a fixed distance out along the direction to it.
            UnkActor* player = other->actor;
            f32 tx = player->field_0x44;
            f32 ty = player->field_0x54;
            f32 tz = player->field_0x64;
            UnkActor* from = fn_7100d7c990(actor);
            f32 dx = tx - from->field_0x44;
            f32 dy = ty - from->field_0x54;
            f32 dz = tz - from->field_0x64;
            f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 1e-4f);
            f32 dirX = dx * inv;
            f32 dirY = dy * inv;
            UnkActor* origin = fn_7100d7c990(actor);
            px = origin->field_0x44 + dirX * 500.0f;
            py = origin->field_0x54 + dirY * 500.0f;
        } else {
            continue;
        }
        minX = Min_l476618(minX, px);
        minY = Min_l476618(minY, py);
        maxX = Max_l476618(maxX, px);
        maxY = Max_l476618(maxY, py);
    }

    if (width >= maxX - minX && width / aspect >= maxY - minY)
        return false;
    return true;
}
