// CGrabComponent (unit 2709): probes the grabbed actor's movement collider along a path and moves the
// out transform's position to where the probe ends; nothing happens when the actor has no character
// movement or its animation ignores collision. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct CGrabComponent;

// The probe's two callbacks into the grab component (this unit).
void fn_710113f708();
void fn_710113f728();

extern f32 g_unk_710164baa0;

struct UnkGrabCallback_l13cf3c {
    CGrabComponent* owner;
    void (*fn)();
};

// What the collider is asked to do.
struct UnkGrabQuery_l13cf3c {
    UnkGrabQuery_l13cf3c() { kind = 2; }

    u32 kind;
    bool field_0x4 = false;
    u32 field_0x8 = 1;
    u32 field_0xc = 0;
    UnkGrabCallback_l13cf3c filter = {};  // 0x10
    UnkGrabCallback_l13cf3c hit = {};     // 0x20
    u64 field_0x30 = 0;
    u64 field_0x38 = 0;
    u64 field_0x40 = 0;
    u64 field_0x48 = 0;
    UnkShape* shape = nullptr;  // 0x50
    u32 field_0x58 = 0;
    u8 field_0x5c[0x10];
    bool field_0x6c = true;
    u32 layers[3] = {};  // 0x70
    u32 field_0x7c = 0;
    f32 field_0x80 = -1.0f;
    u32 field_0x84 = 0;
    bool field_0x88 = true;
};
static_assert(__builtin_offsetof(UnkGrabQuery_l13cf3c, field_0x88) == 0x88, "");

// The path: a radius, where it starts, and two vectors.
struct UnkGrabProbe_l13cf3c {
    f32 radius;
    UnkVec3f start;  // 0x4
    u64 field_0x10[6];
    UnkVec3f field_0x40;
    UnkVec3f field_0x4c;
    f32 field_0x58;
    f32 field_0x5c;
};
static_assert(sizeof(UnkGrabProbe_l13cf3c) == 0x60, "");

struct UnkGrabHit_l13cf3c {
    UnkVec3f position = UnkVec3f(0.0f, 0.0f, 0.0f);
    f32 field_0xc = 0.0f;
    void* field_0x10 = nullptr;
    u64 field_0x18 = 0;
    u32 field_0x20 = 0;
};
static_assert(sizeof(UnkGrabHit_l13cf3c) == 0x28, "");

void fn_7100d4c198(void* collider, UnkGrabQuery_l13cf3c* query, const UnkGrabProbe_l13cf3c* probe,
                   UnkGrabHit_l13cf3c* hit, void* extra);

struct UnkGrabMovement_l13cf3c : UnkReflObject {
    u8 pad_0x8[0x3a8];
    u8 collider[0x18];  // 0x3b0
    u32 layer;          // 0x3c8
};
static_assert(__builtin_offsetof(UnkGrabMovement_l13cf3c, layer) == 0x3c8, "");

struct UnkGrabTarget_l13cf3c {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* animation;  // 0x220
};

void CGrabComponent_ProbeTargetColliderPosition(CGrabComponent* self, UnkXform* out, UnkGrabTarget_l13cf3c* target, const UnkXform* from,
                   const UnkVec3f* a, const UnkVec3f* b, f32 radius) {
    UnkReflObject* object = target->movement;
    auto* movement = object && fn_710009d50c(object->vf_0x0(), fn_7100975b3c())
                         ? static_cast<UnkGrabMovement_l13cf3c*>(object)
                         : nullptr;
    UnkAnimationComponent* animation = target->animation;
    UnkGrabHit_l13cf3c hit;
    if (!movement || !animation || fn_710117b348(animation, fn_7100d07ac8()))
        return;

    UnkGrabQuery_l13cf3c query;
    query.shape = fn_7100976588(reinterpret_cast<UnkFlagged*>(movement));
    if (!query.shape)
        return;
    u32 layer = movement->layer;
    query.layers[0] = layer;
    query.layers[1] = layer;
    query.layers[2] = layer;
    query.filter = {self, fn_710113f708};
    query.hit = {self, fn_710113f728};

    UnkGrabProbe_l13cf3c probe = {};
    probe.field_0x58 = -0.3f;
    probe.field_0x5c = g_unk_710164baa0;
    probe.radius = radius;
    probe.start = from->GetTranslation();
    probe.field_0x40 = *a;
    probe.field_0x4c = *b;
    fn_7100d4c198(movement->collider, &query, &probe, &hit, nullptr);
    out->SetTranslation(hit.position);
}
