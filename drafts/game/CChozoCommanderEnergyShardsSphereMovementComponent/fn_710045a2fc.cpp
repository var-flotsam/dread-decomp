// CChozoCommanderEnergyShardsSphereMovementComponent's unit (tools/tu_map.py unit 671): bursts the sphere into
// eight shards: starts the burst timeline, aims the first shard at the target (on the first burst), spawns
// one shard every eighth of a turn (each turned and placed a little at random) and sets it moving, then
// turns the pattern a twelfth of a half turn for the next burst. Names and layouts are placeholders.
#include <math.h>
#include <stdlib.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;

extern const u64 g_unk_7101649728;                                     // the burst timeline's id
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726c9df8;  // the shard's definition

void* fn_710045a95c(void* owner);  // refl_FindTyped of the shard's movement component.

struct UnkActorDefName_l45a2fc {
    u8 pad_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct alignas(8) UnkActorDefType_l45a2fc {
    u32 id;
};

struct UnkShardMovement_l45a2fc {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 dir_x;  // 0x40
    f32 dir_y;  // 0x44
    f32 dir_z;  // 0x48

    void vf_0x290(UnkActor* owner) {
        reinterpret_cast<void (*)(UnkShardMovement_l45a2fc*, UnkActor*)>(vtable[0x290 / 8])(this, owner);
    }
    void vf_0x2c0() { reinterpret_cast<void (*)(UnkShardMovement_l45a2fc*)>(vtable[0x2c0 / 8])(this); }
};

// The sphere's class: the random spread of each shard's angle (degrees) and of its place.
struct UnkSphereClass_l45a2fc {
    u8 pad_0x0[0x134];
    f32 angle_spread;   // 0x134
    f32 place_spread;   // 0x138
};

struct UnkSphereMovement_l45a2fc {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x598];
    UnkSphereClass_l45a2fc* char_class;  // 0x5d0
    u8 pad_0x5d8[0x18];
    f32 radius;  // 0x5f0
    u8 pad_0x5f4[0x8];
    f32 angle;  // 0x5fc
};
static_assert(__builtin_offsetof(UnkSphereMovement_l45a2fc, angle) == 0x5fc, "layout");

inline f32 DegToRad(f32 deg) {
    return deg * 3.141592653589793 / 180.0;
}

// A random number in [0, 1].
inline f32 RandUnit() {
    return (rand() & 0x7fff) * (1.0 / 32767.0);
}

inline f32 Clamp(const f32& v, const f32& lo, const f32& hi) {
    if (v <= lo)
        return lo;
    if (v >= hi)
        return hi;
    return v;
}

// The weak handle of `target`, created on first use (as GetWeakHandle, game_s2/UnkWeakHandle.h, with the
// new handle's vtable set last).
inline UnkWeakHandle* HandleOf(UnkWeakTarget* target) {
    UnkWeakHandle** slot = target->vf_0x28();
    if (!slot)
        return nullptr;
    UnkWeakHandle* handle = *slot;
    if (!handle) {
        handle = static_cast<UnkWeakHandle*>(fn_71000839a0(0x18, 0x3e));
        handle->ref_count = 0;
        handle->target = target;
        *reinterpret_cast<void***>(handle) = &g_unk_7101a209e0[2];
        AssignRef(slot, handle);
    }
    return handle;
}

void fn_710045a2fc(UnkSphereMovement_l45a2fc* self) {
    if (UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &g_unk_7101649728, &mask.id, false, -1.0f);
    }

    UnkVec3f pos;
    pos = reinterpret_cast<const UnkXform*>(self->owner->field_0x38)->GetTranslation();
    if (self->angle == 0.0f) {
        UnkActor* target = fn_7100d7c990(self->owner);
        UnkVec3f target_pos(target->field_0x44, target->field_0x54, target->field_0x64);
        UnkVec3f dir = Normalized(target_pos - pos);
        self->angle = atan2f(dir.y, dir.x);
    }

    for (u64 i = 0; i < 8; ++i) {
        f32 angle = self->angle + i * 0.7853982f;
        if (self->char_class->angle_spread > 0.0f) {
            f32 spread = DegToRad(self->char_class->angle_spread);
            angle += (2.0f * RandUnit() - 1.0f) * spread;
        }
        f32 c = cosf(angle);
        f32 s = sinf(angle);

        UnkActorDefType_l45a2fc type;
        UnkActorDefFactory* factory = g_unk_71726a0220;
        UnkActorDefName_l45a2fc* name = reinterpret_cast<UnkActorDefName_l45a2fc*>(g_unk_71726c9df8.data);
        ++name->use_count;
        const char* text = reinterpret_cast<UnkActorDefName_l45a2fc*>(g_unk_71726c9df8.data)->text;
        type.id = g_unk_7172a0ba48;
        UnkWeakTarget* shard = reinterpret_cast<UnkWeakTarget*>(fn_710119438c(
            factory, text, &g_unk_71726c9df8, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));

        UnkWeakHandle* handle = HandleOf(shard);
        if (!handle)
            continue;
        ++handle->ref_count;
        if (handle->target) {
            UnkShardMovement_l45a2fc* movement = static_cast<UnkShardMovement_l45a2fc*>(fn_710045a95c(handle->target));
            if (self->char_class->place_spread > 0.0f) {
                f32 radius = Clamp(self->char_class->place_spread, 0.0f, self->radius);
                f32 t = RandUnit();
                UnkVec3f offset(radius * (c * t), radius * (s * t), radius * (0.0f * t));
                pos = pos + offset;
            }
            reinterpret_cast<UnkActor*>(handle->target)->vf_0xd0(&pos, true);
            if (movement) {
                movement->dir_x = c;
                movement->dir_y = s;
                movement->dir_z = 0.0f;
                movement->vf_0x290(self->owner);
                movement->vf_0x2c0();
            }
        }
        if (--handle->ref_count == 0)
            handle->vf_0x10();
    }
    self->angle += 0.2617994f;
}
