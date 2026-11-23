// CSamusMovement's unit (unit 1501): while moving sideways faster than a small threshold, casts the
// movement's capsule from the actor's position along the facing and tells whether it may run along a
// wall it would meet. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCapsuleShape2D.h"
#include "types.h"

extern base::reflection::UnkTunable g_unk_7172a03d80;

// The settings tunable as read here.
struct UnkSamusTunable_l9ea3e4 {
    u8 pad_0x0[0x54];
    bool field_0x54;
};

f32 fn_7100d60238(char* p0);
f32 fn_7100d60230(char* p0);
char* fn_7100d60184(char* p0, f32 f0);

struct UnkMovementOwner_b45a3;
bool fn_71010dca80(UnkMovementOwner_b45a3* self, const void* arg);

// A collision shape: its kind (8 for a capsule) and its offset.
struct UnkShape_l9ea3e4;
struct UnkShapeVtbl_l9ea3e4 {
    void* slots[0x18 / 8];
    s32 (*GetKind)(UnkShape_l9ea3e4* self);
    void* slots_0x20[(0x68 - 0x20) / 8];
    UnkVec3f (*GetOffset)(UnkShape_l9ea3e4* self);
};
struct UnkShape_l9ea3e4 {
    UnkShapeVtbl_l9ea3e4* vtable;
};

// A capsule shape (CAPSULE2D, 0xc0 bytes) built on the stack.
struct UnkCapsule_l9ea3e4;
struct UnkCapsuleVtbl_l9ea3e4 {
    void* slots[0x80 / 8];
    void (*Update)(UnkCapsule_l9ea3e4* self);
};
struct UnkCapsule_l9ea3e4 {
    UnkCapsuleVtbl_l9ea3e4* vtable;
    u8 pad_0x8[0xa0];
    u64 field_0xa8;
    f32 halfLength;  // +0xb0
    f32 radius;      // +0xb4
    u8 pad_0xb8[8];

    UnkCapsule_l9ea3e4() { fn_7100d5ff7c(this); }
    // Sets the capsule's length and radius about its centre and rebuilds it.
    void Set(f32 length, f32 r) {
        field_0xa8 = 0;
        radius = r;
        halfLength = length * 0.5f;
        vtable->Update(this);
    }
    ~UnkCapsule_l9ea3e4() { fn_7100d6006c(this); }
};
static_assert(sizeof(UnkCapsule_l9ea3e4) == 0xc0, "");

// A reference-counted object released through its vtable's +0x10.
struct UnkRefCounted_l9ea3e4;
struct UnkRefCountedVtbl_l9ea3e4 {
    void* slots[0x10 / 8];
    void (*Destroy)(UnkRefCounted_l9ea3e4* self);
};
struct UnkRefCounted_l9ea3e4 {
    UnkRefCountedVtbl_l9ea3e4* vtable;
    u32 refs;
};
struct UnkRefPtr_l9ea3e4 {
    UnkRefCounted_l9ea3e4* ptr;
    UnkRefPtr_l9ea3e4() : ptr(nullptr) {}
    ~UnkRefPtr_l9ea3e4() {
        if (ptr && --ptr->refs == 0)
            ptr->vtable->Destroy(ptr);
    }
};

// What the cast found.
struct UnkCastHit_l9ea3e4 {
    UnkRefPtr_l9ea3e4 first;
    UnkRefPtr_l9ea3e4 second;
    s32 index;
    UnkVec3f field_0x14;
    UnkVec3f field_0x20;
    UnkVec3f field_0x2c;
    UnkVec3f field_0x38;

    UnkCastHit_l9ea3e4()
        : index(-1), field_0x14(0, 0, 0), field_0x20(0, 0, 0), field_0x2c(0, 0, 0), field_0x38(0, 0, 0) {}
};

// The CCharClassToRunNearWallTrack record: how far to look for the wall.
struct UnkRunNearWallTrack_l9ea3e4 {
    u8 pad_0x0[0x54];
    f32 distance;
};

struct UnkSamusMovement_l9ea3e4;
struct UnkCollider_l9ea3e4 {
    u8 pad_0x0[0x158];
    UnkShape_l9ea3e4* shape;
};
struct UnkSamusMovementVtbl_l9ea3e4 {
    void* slots[0x2a8 / 8];
    UnkCollider_l9ea3e4* (*GetCollider)(UnkSamusMovement_l9ea3e4* self);
};
struct UnkSamusMovement_l9ea3e4 {
    UnkSamusMovementVtbl_l9ea3e4* vtable;
    u8 pad_0x8[0x28];
    UnkActor* actor;  // +0x30
    u8 pad_0x38[8];
    f32 field_0x40;
    u8 pad_0x44[0x7a0];
    bool field_0x7e4;
};

static inline UnkVec3f Position(const UnkActor* actor) {
    return UnkVec3f(actor->field_0x44, actor->field_0x54, actor->field_0x64);
}

bool fn_71009ed53c(UnkSamusMovement_l9ea3e4* self, const UnkVec3f* from, const UnkVec3f* dir,
                   UnkCapsule_l9ea3e4* capsule, UnkCastHit_l9ea3e4* hit, f32 distance);

struct UnkSamusMove_b50a5;

bool fn_71009ea3e4(UnkSamusMove_b50a5* move, f32 speed) {
    UnkSamusMovement_l9ea3e4* self = reinterpret_cast<UnkSamusMovement_l9ea3e4*>(move);
    if (!reinterpret_cast<const UnkSamusTunable_l9ea3e4*>(&g_unk_7172a03d80)->field_0x54)
        return false;
    f32 side = self->field_0x40;
    if (side + 0.1f >= 0.0f && side - 0.1f <= 0.0f)
        return false;

    UnkShape_l9ea3e4* shape = self->vtable->GetCollider(self)->shape;
    UnkShape_l9ea3e4* capsuleShape = shape->vtable->GetKind(shape) == 8 ? shape : nullptr;
    if (!capsuleShape)
        return false;

    UnkCapsule_l9ea3e4 capsule;
    f32 length = fn_7100d60238(reinterpret_cast<char*>(capsuleShape)) * 2.0f;
    f32 radius = fn_7100d60230(reinterpret_cast<char*>(capsuleShape));
    capsule.Set(length, radius);
    fn_7100d60184(reinterpret_cast<char*>(&capsule), 0.999f);

    UnkVec3f from = Position(self->actor) + capsuleShape->vtable->GetOffset(capsuleShape);

    UnkCastHit_l9ea3e4 hit;
    f32 distance = self->field_0x7e4 ? 5.0f : 15.0f;
    UnkTrackQuery query = {fn_7100c53d88(), nullptr, -1.0f};
    fn_710117b08c(static_cast<UnkAnimationComponent*>(self->actor->field_0x220), &query);
    if (query.result)
        distance = reinterpret_cast<UnkRunNearWallTrack_l9ea3e4*>(query.result)->distance;

    f32 facing = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38)).m[2];
    f32 sign = (facing > 0.0f) - (facing < 0.0f);
    UnkVec3f dir(sign, sign * 0.0f, sign * 0.0f);

    bool free = false;
    if (fn_71009ed53c(self, &from, &dir, &capsule, &hit, distance))
        free = !fn_71010dca80(reinterpret_cast<UnkMovementOwner_b45a3*>(self), &hit);
    return free;
}
