// CUsableComponent unit (tools/tu_map.py, unit 1607): picks the side id for the player's position (front or
// back, by the facing of the player's transform, and near, middle or far by the distance to the owner), then
// builds "<prefix>_<side>" and, when the player's animation has that name, makes it the animation id.
// Names and layouts are placeholders.
#include <math.h>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CAnimationComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkStruct_7101176ea8;

// The usable tunables: distances and the side ids.
struct UnkUsableSides_laa0810 {
    u8 pad_0x0[0x28];
    f32 min_distance;  // 0x28
    f32 near_distance; // 0x2c
    CStrId front_near; // 0x30
    CStrId back_near;  // 0x38
    f32 mid_distance;  // 0x40
    u8 pad_0x44[0x4];
    CStrId front_mid;  // 0x48
    CStrId back_mid;   // 0x50
    CStrId front_far;  // 0x58
    CStrId back_far;   // 0x60
};
// The usable tunables object (declared with the save station's view of it).
struct UnkSaveStationTunables_ovb0118;

struct UnkUsableOwner_laa0810 {
    u8 pad_0x0[0x44];
    f32 x;
};

struct UnkUsable_laa0810 {
    u8 pad_0x0[0x30];
    UnkUsableOwner_laa0810* owner;
};

// Room for a string id built by the out-of-line constructor (fn_7100000850); it has no destructor, since
// its string is always handed on.
union UnkStrIdSlot_laa0810 {
    CStrId id;
    UnkStrIdSlot_laa0810() {}
    ~UnkStrIdSlot_laa0810() {}
};

struct UnkUsableVec_laa0810 {
    f32 x;
    f32 y;
    f32 z;
};

static inline f32 Dot(const UnkUsableVec_laa0810& a, const UnkUsableVec_laa0810& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// The id's string, counting the read.
inline const CRntString& IdString(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

void fn_7100aa0810(UnkUsable_laa0810* self, const CStrId* prefix, CStrId* side, CStrId* animation_id) {
    auto* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    f32 x = self->owner->x;
    f32 distance = fabsf(x - player->field_0x44);
    auto& sides = reinterpret_cast<UnkUsableSides_laa0810&>(g_unk_71727531f0);
    if (distance > sides.min_distance) {
        f32 offset = x - player->field_0x44;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(player->field_0x38));
        // The player's facing: the offset along x against the transform's third column.
        UnkUsableVec_laa0810 forward = {m.m[2], m.m[6], m.m[10]};
        UnkUsableVec_laa0810 to_owner = {offset, 0.0f, 0.0f};
        if (Dot(to_owner, forward) > 0.0f) {
            if (distance <= sides.near_distance)
                *side = sides.front_near;
            else if (distance <= sides.mid_distance)
                *side = sides.front_mid;
            else
                *side = sides.front_far;
        } else {
            if (distance <= sides.near_distance)
                *side = sides.back_near;
            else if (distance <= sides.mid_distance)
                *side = sides.back_mid;
            else
                *side = sides.back_far;
        }
    }
    const UnkStringData* chosen = side->data;
    if (chosen == &g_unk_7101d47108->empty)
        return;

    UnkBufferString<0x80, g_unk_7101a1b750> name;
    fn_710007eb6c(name.Str(), prefix);
    fn_710007f564(name.Str(), "_");
    fn_710007f3b4(name.Str(), &IdString(*side));
    auto* animation = static_cast<UnkAnimationComponent*>(player->field_0x220);
    bool found;
    {
        CRntString copy(*name.Str());
        u64 key;
        fn_7100066630(&key, &copy);
        found = fn_7101176e78(reinterpret_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(key), nullptr);
    }
    if (found) {
        UnkStrIdSlot_laa0810 slot;
        CStrId_ConstructFromRntString(&slot.id, *name.Str(), true);
        animation_id->Reset();
        animation_id->data = slot.id.data;
    }
}
