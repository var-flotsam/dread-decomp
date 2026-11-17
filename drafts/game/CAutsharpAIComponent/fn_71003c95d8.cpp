// CAutsharpAIComponent unit (tools/tu_map.py unit 572): fn_71003c95d8, in states 2 and 3, finds the bombs
// (entities of fn_7100aa9ecc's list with a CBombMovement) within the actor's radius in the plane, marks the
// component hit and makes each bomb explode at its position. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBombMovement.h"
#include "game/CMovementComponent.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkBombExplodeMovement_ovb0078;
struct UnkBombTargetActor_ovb0078;
void fn_7100aaa7d8(UnkBombExplodeMovement_ovb0078* self, UnkBombTargetActor_ovb0078* actor, bool hit);

struct UnkAutsharpBombMovement_l3c95d8 : UnkReflObject {
    u8 pad_0x8[0x1d0];
    CVector3D position;  // 0x1d8
    u8 pad_0x1e4[0x24];
    CVector3D velocity;  // 0x208
};

struct UnkAutsharpActor_l3c95d8 : UnkReflObject {
    u8 pad_0x8[0x3c];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x188];
    UnkAutsharpBombMovement_l3c95d8* movement;  // 0x1f0
    u8 pad_0x1f8[0x184];
    f32 radius;  // 0x37c
};

static inline CVector3D Pos(const UnkAutsharpActor_l3c95d8* a) {
    return {a->x, a->y, a->z};
}

// The actor's position projected on the plane.
static inline void PlanePos(CVector2D* out, const UnkAutsharpActor_l3c95d8* a) {
    CVector3D pos = Pos(a);
    fn_710007b430(out, &pos);
}

// The object if it is of the class, else null.
template <class T>
inline T* Cast(T* o, const base::reflection::CClass* klass) {
    if (o && fn_710009d50c(o->vf_0x0(), klass))
        return o;
    return nullptr;
}

struct UnkAutsharpBombTarget_l3c95d8 {
    u8 pad_0x0[0x18];
    UnkAutsharpActor_l3c95d8* entity;  // 0x18
};

struct UnkAutsharpBombEntry_l3c95d8 {
    u8 pad_0x0[0x10];
    UnkAutsharpBombTarget_l3c95d8* target;  // 0x10
};

struct UnkAutsharpBombList_l3c95d8 {
    UnkAutsharpBombEntry_l3c95d8** data;
    u32 count;
};

static inline UnkAutsharpBombList_l3c95d8* List() {
    return reinterpret_cast<UnkAutsharpBombList_l3c95d8*>(fn_7100aa9ecc());
}

static inline UnkAutsharpBombEntry_l3c95d8** End(UnkAutsharpBombList_l3c95d8* list) {
    return list->data + list->count;
}

struct UnkAutsharpAI_l3c95d8 {
    u8 pad_0x0[0x30];
    UnkAutsharpActor_l3c95d8* actor;  // 0x30
    u8 pad_0x38[0x146a0];
    u32 state;  // 0x146d8
    u8 pad_0x146dc[0x60];
    bool bomb_hit;  // 0x1473c
};
static_assert(__builtin_offsetof(UnkAutsharpAI_l3c95d8, state) == 0x146d8, "layout");
static_assert(__builtin_offsetof(UnkAutsharpAI_l3c95d8, bomb_hit) == 0x1473c, "layout");

void fn_71003c95d8(UnkAutsharpAI_l3c95d8* self) {
    if ((self->state & ~1u) != 2)
        return;
    CVector2D here;
    PlanePos(&here, self->actor);
    for (UnkAutsharpBombEntry_l3c95d8** it = List()->data; it != End(List()); ++it) {
        UnkAutsharpActor_l3c95d8* e = (*it)->target->entity;
        UnkAutsharpActor_l3c95d8* entity = Cast(e, fn_7100d70c20());
        UnkAutsharpBombMovement_l3c95d8* movement = entity->movement;
        if (!movement || !fn_710009d50c(movement->vf_0x0(), fn_7100aa9174()))
            continue;
        CVector2D there;
        PlanePos(&there, entity);
        f32 dx = here.x - there.x;
        f32 dy = here.y - there.y;
        f32 radius = self->actor->radius;
        if (dx * dx + dy * dy > radius * radius)
            continue;
        self->bomb_hit = true;
        fn_7100aaa7d8(reinterpret_cast<UnkBombExplodeMovement_ovb0078*>(movement),
                      reinterpret_cast<UnkBombTargetActor_ovb0078*>(entity), true);
        const CVector3D& zero = g_unk_7101647790;
        movement->velocity.x += zero.x;
        movement->velocity.y += zero.y;
        movement->velocity.z += zero.z;
        CVector3D at = {zero.x + movement->position.x, zero.y + movement->position.y,
                        zero.z + movement->position.z};
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(movement),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(&at), true);
    }
}
