// CChozoCommanderEnergyShardsAttack's unit (tools/tu_map.py unit 647): launches the energy shards. With
// a shard actor already out it is released and the attack moves on; otherwise the shard actor is spawned
// and held through a weak handle, placed at the owner (or at the owner's launch point) facing the middle
// of the commander's range, and its shard is aimed there. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkChozoCommanderAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkMatrix44Words {
    u32 m[16];

    UnkMatrix44Words() {}
    UnkMatrix44Words(const UnkMatrix44Words& other);
};

UnkMatrix44Words CopyMatrixWithoutTranslation(const UnkMatrix44Words* src);
UnkVec3f fn_7100076acc(const UnkXform* x);  // The rotation angles of a transform.
UnkVec3f fn_710007a7f0(const UnkVec3f& dir);  // Direction to rotation angles.

// The shard actor's definition and the launch point's key.
extern CStrId g_unk_71726c7590 __attribute__((visibility("hidden")));
extern char g_unk_7101649450[];

struct UnkEntity;

// The shard the spawned actor carries.
struct UnkEnergyShard_42ac8c {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkVec3f direction;  // 0x40
    u8 pad_0x4c[0x58c];
    f32 target_x;  // 0x5d8
    f32 target_y;  // 0x5dc
    f32 target_z;  // 0x5e0

    void SetOwner(UnkActor* owner) {
        reinterpret_cast<void (*)(UnkEnergyShard_42ac8c*, UnkActor*)>(vtable[0x290 / 8])(this, owner);
    }
    void Release() { reinterpret_cast<void (*)(UnkEnergyShard_42ac8c*)>(vtable[0x2c0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEnergyShard_42ac8c, target_x) == 0x5d8, "UnkEnergyShard_42ac8c layout");

// The shard of an actor, or null.
UnkEnergyShard_42ac8c* fn_710042b0bc(UnkWeakTarget* actor);

struct UnkShardsTuning_42ac8c {
    u8 pad_0x0[0x100];
    f32 height;  // 0x100
};

struct UnkShardsAttack_42ac8c {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x40];
    UnkShardsTuning_42ac8c* tuning;  // 0x50
    UnkWeakHandle* shard;  // 0x58: the spawned shard actor
};

// The spawn's type argument.
struct UnkShardDefType_42ac8c {
    u32 id;
};

// Turns an actor to the angles `angles` (vtable slot 0xd8; the callers pass true).
static inline void SetAngles(UnkActor* actor, const UnkVec3f* angles, bool flag) {
    reinterpret_cast<void (*)(UnkActor*, const UnkVec3f*, bool)>(actor->vtable[0xd8 / 8])(actor, angles, flag);
}

// The x axis of a rotation matrix (its first column).
static inline UnkVec3f XAxis(const UnkMatrix44Words& rotation) {
    const UnkMatrix44& m = reinterpret_cast<const UnkMatrix44&>(rotation);
    return UnkVec3f(m.m[0], m.m[4], m.m[8]);
}

void fn_710042ac8c(UnkShardsAttack_42ac8c* self) {
    if (self->shard && self->shard->target) {
        UnkEnergyShard_42ac8c* shard = fn_710042b0bc(self->shard->target);
        if (!shard)
            return;
        shard->Release();
        fn_7100426b54(reinterpret_cast<UnkChozoCommanderAttack*>(self), 0x2376ca3f9e17e420, false);
        return;
    }

    UnkActorDefFactory* factory = g_unk_71726a0220;
    base::global::UnkStringData* name = g_unk_71726c7590.data;
    ++name->reads;
    const char* path = reinterpret_cast<const char*>(&name->String());
    UnkShardDefType_42ac8c type;
    type.id = g_unk_7172a0ba48;
    UnkWeakTarget* actor = reinterpret_cast<UnkWeakTarget*>(
        fn_710119438c(factory, path, &g_unk_71726c7590,
                      reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    AssignRef(&self->shard, GetWeakHandle(actor));

    // The owner's AI, when it is a commander's.
    UnkReflObject* commander = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (commander && !fn_710009d50c(commander->vf_0x0(), fn_710044e708()))
        commander = nullptr;
    if (!self->shard || !commander || !self->shard->target)
        return;

    UnkActor* owner = self->owner;
    UnkVec3f position(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    const UnkMatrix44& xform = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    UnkVec3f angles = fn_710007a7f0(UnkVec3f(xform.m[2], xform.m[6], xform.m[10]));
    // The ends of the commander's range.
    f32 start;
    f32 end;
    CAIComponent_GetPathFirstLegStartX(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(commander), &start);
    fn_71010fc284(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(commander), &end);
    f32 x = start + (end - start) * 0.5f;
    f32 y = self->owner->field_0x54 + self->tuning->height;
    UnkVec3f dir(x - position.x, y - position.y, 0.0f - position.z);
    f32 length = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

    UnkMatrix44 launch = {};
    launch.m[15] = 1.0f;
    launch.m[10] = 1.0f;
    launch.m[5] = 1.0f;
    launch.m[0] = 1.0f;
    if (fn_71011735e8(static_cast<UnkEntity*>(self->owner->field_0x228), g_unk_7101649450, &launch, 0)) {
        // From the launch point, along its x axis.
        position = UnkVec3f(launch.m[3], launch.m[7], launch.m[11]);
        angles = fn_7100076acc(reinterpret_cast<const UnkXform*>(&launch));
        dir = XAxis(CopyMatrixWithoutTranslation(reinterpret_cast<const UnkMatrix44Words*>(&launch)));
    } else {
        f32 inv = 1.0f / (length + 0.0001f);
        dir = UnkVec3f(dir.x * inv, dir.y * inv, dir.z * inv);
    }
    position.z = 0.0f;
    reinterpret_cast<UnkActor*>(self->shard->target)->vf_0xd0(&position, true);
    SetAngles(reinterpret_cast<UnkActor*>(self->shard->target), &angles, true);

    UnkEnergyShard_42ac8c* shard = fn_710042b0bc(self->shard ? self->shard->target : nullptr);
    if (shard) {
        shard->direction = dir;
        shard->SetOwner(self->owner);
        shard->target_x = x;
        shard->target_y = y;
        shard->target_z = 0.0f;
    }
}
