// CChozoWarriorAIComponent unit (tools/tu_map.py unit 733): picks, of three attack timelines, the one whose
// travel is nearest the distance to cover, and stores the ratio of that distance to the travel.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CAIComponent/CAIComponent_ReachRadius.cpp defines it.
struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

// As src/game/CChozoWarriorDeflectorShieldAttack/CChozoWarriorDeflectorShieldAttack_Height_ovb0130.cpp
// defines them.
struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

// As src/game/CAnimationComponent/CAnimationComponent_Tracks_ovb0009.cpp defines it.
struct UnkTimelineId {
    u64 value;
};
UnkTimelineId fn_7101177be0(UnkAnimationComponent* self, u64 key);

namespace base::reflection {
struct UnkReflObject;
}
bool fn_71010fc554(base::reflection::UnkReflObject* component, f32* out);
bool fn_71010fc5fc(base::reflection::UnkReflObject* component, f32* out);

// As src/game/CChozoWarriorAIComponent/fn_710049cbd8.cpp defines it.
struct UnkSelf;
bool fn_710049cbd8(UnkSelf* self);

// The three attacks' timeline names.
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726d12d0;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726d12d8;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726d12e0;

struct UnkChozoWarriorTunables_l49c59c {
    u8 pad_0x0[0x800];
    f32 field_0x800;
};

struct UnkChozoWarriorAI_l49c59c;

struct UnkChozoWarriorAIVtbl_l49c59c {
    u8 pad_0x0[0x3c8];
    f32 (*vf_0x3c8)(UnkChozoWarriorAI_l49c59c* self);
    u8 pad_0x3d0[0x840 - 0x3d0];
    f32 (*vf_0x840)(UnkChozoWarriorAI_l49c59c* self);
};

struct UnkChozoWarriorAI_l49c59c {
    UnkChozoWarriorAIVtbl_l49c59c* vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x149ac - 0x38];
    f32 ratio;  // 0x149ac
    u8 pad_0x149b0[0x149d0 - 0x149b0];
    UnkChozoWarriorTunables_l49c59c* tunables;  // 0x149d0
};
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l49c59c, tunables) == 0x149d0, "layout");

static inline const UnkMatrix44* Transform(UnkActor* actor) {
    return reinterpret_cast<const UnkMatrix44*>(actor->field_0x38);
}

// The attack's reach past its base position, less half the target's radius.
static inline f32 Offset(UnkChozoWarriorAI_l49c59c* self, void* tracked) {
    UnkAIReach reach = fn_7100977b90(tracked);
    return self->vtable->vf_0x3c8(self) + reach.radius * -0.5f;
}

static inline f32 Front(UnkChozoWarriorAI_l49c59c* self) {
    f32 base;
    fn_71010fc284(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(self), &base);
    f32 offset = Offset(self, self->owner->field_0x1f8);
    return base + offset;
}

static inline f32 Back(UnkChozoWarriorAI_l49c59c* self) {
    f32 base;
    fn_71010fc31c(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(self), &base);
    f32 offset = Offset(self, self->owner->field_0x1f8);
    return base - offset;
}

// How far the timeline `name` moves the owner forward.
static inline f32 Travel(UnkChozoWarriorAI_l49c59c* self, UnkStruct_7101176ea8* actions, const CStrId& name) {
    u64 key = name.data->Key();
    UnkTimelineId id = fn_7101177be0(static_cast<UnkAnimationComponent*>(self->owner->field_0x220),
                                     0xf4061ff25a2163d6);
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176e0c(actions, key, id.value));
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    return motion.m[11];
}

CStrId fn_710049c59c(UnkChozoWarriorAI_l49c59c* self) {
    CStrId result(g_unk_710162e2ed, true);
    UnkActor* owner = self->owner;
    UnkStruct_7101176ea8* actions = static_cast<UnkStruct_7101176ea8*>(owner->field_0x220);
    void* tracked = owner->field_0x1f8;
    UnkActor* target = fn_7100d7c990(owner);
    if (actions && tracked && target) {
        f32 limit;
        f32 edge;
        if (fn_710002614c(Transform(self->owner)).m[2] >= 0.0f) {
            fn_71010fc554(reinterpret_cast<base::reflection::UnkReflObject*>(self), &limit);
            edge = Front(self);
        } else {
            fn_71010fc5fc(reinterpret_cast<base::reflection::UnkReflObject*>(self), &limit);
            edge = Back(self);
        }
        f32 room = __builtin_fabsf(edge - self->owner->field_0x44) - self->vtable->vf_0x840(self);
        f32 dx = target->field_0x44 - self->owner->field_0x44;
        if (fn_710049cbd8(reinterpret_cast<UnkSelf*>(self))) {
            f32 sign = (dx > 0.0f) - (dx < 0.0f);
            dx += self->tunables->field_0x800 * sign;
        }
        f32 distance = __builtin_fabsf(dx);
        if (limit < distance)
            distance = limit;
        f32 offset = Offset(self, tracked);
        f32 want = distance <= offset ? offset : (distance >= room ? room : distance);

        const CStrId* names[] = {&g_unk_71726d12d0, &g_unk_71726d12d8, &g_unk_71726d12e0};
        f32 best = 3.4028235e38f;
        f32 travel = 0.0f;
        for (const CStrId* name : names) {
            f32 t = Travel(self, actions, *name);
            f32 diff = __builtin_fabsf(t - want);
            if (diff < best) {
                result = *name;
                best = diff;
                travel = t;
            }
        }
        self->ratio = want / travel;
    }
    return result;
}
