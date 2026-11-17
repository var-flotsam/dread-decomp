// CVulkranAIComponent unit (tools/tu_map.py unit 1084): spawns a magma ball. When spawning is enabled
// (g_unk_7101cb1dc8) it loads the ball's actor definition through the actordef factory, checks that
// it is an entity whose movement component is a CVulkranMagmaBallMovementComponent of the char class
// CCharClassVulkranMagmaBallMovementComponent, places it ahead of the owner and turned along `angle`,
// and hands the component its speed, direction and owner. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU341.h"
#include "game/UnkTU344.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflObject;

// Whether magma balls may be spawned.
extern bool g_unk_7101cb1dc8;

struct alignas(8) UnkActorDefType_l6ddafc {
    u32 id;
};

struct UnkVulkranParams_l6ddafc {
    u8 pad_0x0[0x6c8];
    f32 spawn_distance;  // 0x6c8
    u8 pad_0x6cc[0x14];
    u32 mode;  // 0x6e0
};

// The id of the magma ball's actor definition.
CStrId* fn_71006daba0(UnkVulkranParams_l6ddafc* params);

struct UnkVulkranOwner_l6ddafc {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38: the position is its translation

    UnkVec3f Position() const { return UnkVec3f(transform.m[3], transform.m[7], transform.m[11]); }
};

struct UnkVulkranAI_l6ddafc {
    u8 pad_0x0[0x30];
    UnkVulkranOwner_l6ddafc* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkVulkranParams_l6ddafc* params;  // 0x146c8
};
static_assert(__builtin_offsetof(UnkVulkranAI_l6ddafc, params) == 0x146c8, "layout");

void fn_71006dddac(UnkVulkranAI_l6ddafc* self);

struct UnkMagmaBallMovement_l6ddafc : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x28];
    UnkVec3f dir;  // 0x40
    u8 pad_0x4c[0x30];
    f32 speed;  // 0x7c
    u8 pad_0x80[0x88];
    f32 field_0x108;
    u8 pad_0x10c[0x464];
    bool field_0x570;

    void SetOwner(UnkVulkranOwner_l6ddafc* owner) {
        reinterpret_cast<void (*)(UnkMagmaBallMovement_l6ddafc*, UnkVulkranOwner_l6ddafc*)>(
            (*reinterpret_cast<void***>(this))[0x290 / 8])(this, owner);
    }
    void vf_0x2c0() {
        reinterpret_cast<void (*)(UnkMagmaBallMovement_l6ddafc*)>((*reinterpret_cast<void***>(this))[0x2c0 / 8])(
            this);
    }
};
static_assert(__builtin_offsetof(UnkMagmaBallMovement_l6ddafc, field_0x570) == 0x570, "layout");

struct UnkMagmaBallActor_l6ddafc : UnkReflObject {
    u8 pad_0x8[0x1e8];
    UnkReflObject* movement;  // 0x1f0

    // vtable slot 0xd0: places the actor at `pos`.
    void SetPosition(const UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkMagmaBallActor_l6ddafc*, const UnkVec3f*, bool)>(
            (*reinterpret_cast<void***>(this))[0xd0 / 8])(this, pos, flag);
    }
    // vtable slot 0xd8: turns the actor to `angles`.
    void SetAngles(const UnkVec3f* angles, bool flag) {
        reinterpret_cast<void (*)(UnkMagmaBallActor_l6ddafc*, const UnkVec3f*, bool)>(
            (*reinterpret_cast<void***>(this))[0xd8 / 8])(this, angles, flag);
    }
};
static_assert(__builtin_offsetof(UnkMagmaBallActor_l6ddafc, movement) == 0x1f0, "layout");

// The id's string, which the factory takes as the definition's path, counting the read.
static inline const char* PathOf(CStrId* id) {
    ++id->data->reads;
    return reinterpret_cast<const char*>(&id->data->String());
}

// Whether `obj` is of the class `Getter` describes, or derives from it.
template <const CClass* (*Getter)()>
static inline bool IsKindOf(const UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), Getter());
}

bool fn_71006ddafc(UnkVulkranAI_l6ddafc* self, f32 speed, f32 angle, f32 field_0x108,
                   UnkMagmaBallMovement_l6ddafc** out_ball) {
    if (!g_unk_7101cb1dc8)
        return false;

    UnkActorDefFactory* factory = g_unk_71726a0220;
    const char* path = PathOf(fn_71006daba0(self->params));
    CStrId* id = fn_71006daba0(self->params);
    UnkActorDefType_l6ddafc type;
    type.id = g_unk_7172a0ba48;
    auto* actor = reinterpret_cast<UnkMagmaBallActor_l6ddafc*>(
        fn_710119438c(factory, path, id, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    if (!actor || !IsKindOf<fn_7100d70c20>(actor)) {
        *out_ball = nullptr;
        return false;
    }

    UnkReflObject* movement = actor->movement;
    *out_ball = movement && IsKindOf<fn_71006df3d4>(movement)
                    ? static_cast<UnkMagmaBallMovement_l6ddafc*>(movement)
                    : nullptr;
    UnkReflObject* char_class = *out_ball ? (*out_ball)->char_class : nullptr;
    if (!char_class || !IsKindOf<fn_71006defd8>(char_class) || !*out_ball) {
        fn_7100232c68(actor);
        *out_ball = nullptr;
        return false;
    }

    if (self->params->mode == 1)
        fn_71006dddac(self);
    UnkVulkranOwner_l6ddafc* owner = self->owner;
    UnkVec3f pos = owner->Position();
    f32 distance = self->params->spawn_distance;
    const UnkMatrix44& m = fn_710002614c(&owner->transform);
    UnkVec3f ahead(distance * m.m[1], distance * m.m[5], distance * m.m[9]);
    pos.x += ahead.x;
    pos.y += ahead.y;
    pos.z += ahead.z;
    actor->SetPosition(&pos, true);

    UnkVec3f dir(cosf(angle), sinf(angle), 0.0f);
    UnkVec3f angles = fn_710007a7f0(dir);
    actor->SetAngles(&angles, true);

    (*out_ball)->field_0x570 = false;
    (*out_ball)->speed = speed;
    (*out_ball)->field_0x108 = field_0x108;
    (*out_ball)->dir = dir;
    (*out_ball)->SetOwner(self->owner);
    (*out_ball)->vf_0x2c0();
    return true;
}
