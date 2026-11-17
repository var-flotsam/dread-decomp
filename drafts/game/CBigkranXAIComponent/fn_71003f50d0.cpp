// build-flags: no-strict-aliasing
// CBigkranXAIComponent's unit: spawns a magma ball. Loads the ball's actor definition through the actordef
// factory, checks that it is an entity whose movement is a CVulkranMagmaBallMovementComponent with its char
// class, places it above the owner, aims it along `angle` and starts it. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU341.h"
#include "game/UnkTU344.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;

char* fn_71003f6358(char* p0);

extern const u64 g_unk_71016493a8;  // the id of the owner's node the ball starts from

struct UnkActorDefFactory;
struct UnkPowerUpLifeType_b52;

struct UnkDefName_l3f50d0 {
    u8 unk_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct alignas(8) UnkDefType_l3f50d0 {
    u32 id;
};

// The object at +0x149b8: names the ball's definition (fn_71003f6358) and its spawn height.
struct UnkMagmaBallSource_l3f50d0 {
    u8 pad_0x0[0x618];
    f32 height;  // 0x618
};

struct UnkBigkranXOwner_l3f50d0 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x228 - 0x78];
    void* model_updater;  // 0x228
};

struct UnkBigkranXAI_l3f50d0 {
    u8 pad_0x0[0x30];
    UnkBigkranXOwner_l3f50d0* owner;  // 0x30
    u8 pad_0x38[0x149b8 - 0x38];
    UnkMagmaBallSource_l3f50d0* source;  // 0x149b8
};

struct UnkMagmaBallActor_l3f50d0;
struct UnkMagmaBallActorVtbl_l3f50d0 {
    const CType* (*vf_0x0)(UnkMagmaBallActor_l3f50d0* self);
    void* pad_0x8[0xd0 / 8 - 1];
    void (*vf_0xd0)(UnkMagmaBallActor_l3f50d0* self, const UnkVec3f* position, bool apply);
    void (*vf_0xd8)(UnkMagmaBallActor_l3f50d0* self, const UnkVec3f* angles, bool apply);
};

struct UnkMagmaBallClass_l3f50d0;
struct UnkMagmaBallClassVtbl_l3f50d0 {
    const CType* (*vf_0x0)(UnkMagmaBallClass_l3f50d0* self);
};
struct UnkMagmaBallClass_l3f50d0 {
    UnkMagmaBallClassVtbl_l3f50d0* vtable;
};

struct UnkMagmaBall_l3f50d0;
struct UnkMagmaBallVtbl_l3f50d0 {
    const CType* (*vf_0x0)(UnkMagmaBall_l3f50d0* self);
    void* pad_0x8[0x290 / 8 - 1];
    void (*vf_0x290)(UnkMagmaBall_l3f50d0* self, UnkBigkranXOwner_l3f50d0* owner);
    void* pad_0x298[(0x2c0 - 0x298) / 8];
    void (*vf_0x2c0)(UnkMagmaBall_l3f50d0* self);
};

// The ball's CVulkranMagmaBallMovementComponent.
struct UnkMagmaBall_l3f50d0 {
    UnkMagmaBallVtbl_l3f50d0* vtable;
    u8 pad_0x8[0x8];
    UnkMagmaBallClass_l3f50d0* char_class;  // 0x10
    u8 pad_0x18[0x40 - 0x18];
    UnkVec3f direction;  // 0x40
    u8 pad_0x4c[0x7c - 0x4c];
    f32 field_0x7c;
    u8 pad_0x80[0x108 - 0x80];
    f32 field_0x108;
    u8 pad_0x10c[0x570 - 0x10c];
    bool field_0x570;
};

struct UnkMagmaBallActor_l3f50d0 {
    UnkMagmaBallActorVtbl_l3f50d0* vtable;
    u8 pad_0x8[0x1f0 - 0x8];
    UnkMagmaBall_l3f50d0* movement;  // 0x1f0
};

// Whether `object` is of the class `Getter` returns (its type is asked for first).
template <const base::reflection::CClass* (*Getter)(), class T>
static inline bool IsA_l3f50d0(T* object) {
    const CType* type = object->vtable->vf_0x0(object);
    return fn_710009d50c(type, Getter());
}

// fn_710007a7f0 reads the direction in place (UnkTU54.h declares it by value).
static inline UnkVec3f Angles_l3f50d0(const UnkVec3f& direction) {
    return reinterpret_cast<UnkVec3f (*)(const UnkVec3f&)>(fn_710007a7f0)(direction);
}

static inline CStrId* BallDef_l3f50d0(UnkBigkranXAI_l3f50d0* self) {
    return reinterpret_cast<CStrId*>(fn_71003f6358(reinterpret_cast<char*>(self->source)));
}

static inline UnkDefName_l3f50d0* Name_l3f50d0(const CStrId& id) {
    return reinterpret_cast<UnkDefName_l3f50d0*>(id.data);
}

bool fn_71003f50d0(UnkBigkranXAI_l3f50d0* self, UnkMagmaBall_l3f50d0** out, f32 speed, f32 angle, f32 field_0x108) {
    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkDefType_l3f50d0 type;
    CStrId* def = BallDef_l3f50d0(self);
    ++Name_l3f50d0(*def)->use_count;
    const char* text = Name_l3f50d0(*def)->text;
    CStrId* id = BallDef_l3f50d0(self);
    type.id = g_unk_7172a0ba48;
    auto* actor = reinterpret_cast<UnkMagmaBallActor_l3f50d0*>(
        fn_710119438c(factory, text, id, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    if (!actor || !IsA_l3f50d0<fn_7100d70c20>(actor)) {
        *out = nullptr;
        return false;
    }

    UnkMagmaBall_l3f50d0* ball = actor->movement;
    if (ball && !IsA_l3f50d0<fn_71006df3d4>(ball))
        ball = nullptr;
    *out = ball;
    if (!ball || !ball->char_class || !IsA_l3f50d0<fn_71006defd8>(ball->char_class) || !*out) {
        fn_7100232c68(actor);
        *out = nullptr;
        return false;
    }

    // Start above the owner, at its node when it has one.
    UnkBigkranXOwner_l3f50d0* owner = self->owner;
    UnkVec3f position(owner->transform.m[3], owner->transform.m[7], owner->transform.m[11]);
    f32 height = self->source->height;
    const UnkMatrix44& m = fn_710002614c(&owner->transform);
    // The owner's up axis, scaled by the height.
    UnkVec3f offset(height * m.m[1], height * m.m[5], height * m.m[9]);
    position.x = offset.x + position.x;
    position.y = offset.y + position.y;
    position.z = offset.z + position.z;
    if (self->owner && self->owner->model_updater)
        fn_710117364c(self->owner->model_updater, &g_unk_71016493a8,
                      reinterpret_cast<UnkGrappleAnswer_b50*>(&position));
    actor->vtable->vf_0xd0(actor, &position, true);

    UnkVec3f direction(cosf(angle), sinf(angle), 0.0f);
    UnkVec3f angles = Angles_l3f50d0(direction);
    actor->vtable->vf_0xd8(actor, &angles, true);

    (*out)->field_0x570 = false;
    (*out)->field_0x7c = speed;
    (*out)->field_0x108 = field_0x108;
    (*out)->direction = direction;
    (*out)->vtable->vf_0x290(*out, self->owner);
    (*out)->vtable->vf_0x2c0(*out);
    return true;
}
