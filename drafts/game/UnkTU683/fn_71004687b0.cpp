// Unit 683 (CChozoRobotSoldierDashSlashAttack): the attack's update. After the base update, until the
// request has gone out, sends it once the owner has moved far enough from where it started, or once
// another actor ahead of it (or the point a moving one heads for) comes within 400 units.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

struct UnkRefl_fn_7100468bb8;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_7100468bb8>(void*);

struct UnkUnit1444_o0309;
UnkVec3f fn_7100973658(UnkUnit1444_o0309* self);
bool fn_7101178340(UnkAnimationComponent* self, u64 key);

// An entity in the scene: its actor at +0x18.
struct UnkEntity_l4687b0 {
    u8 pad_0x0[0x18];
    UnkActor* actor;
};

// A handle to an entity (null when empty).
struct UnkEntityRef_l4687b0 {
    u8 pad_0x0[0x10];
    UnkEntity_l4687b0* entity;
};

static inline UnkEntity_l4687b0* Get_l4687b0(UnkEntityRef_l4687b0* ref) {
    return ref ? ref->entity : nullptr;
}

struct UnkEntityList_l4687b0 {
    UnkEntityRef_l4687b0** data;
    u32 count;
};
UnkEntityList_l4687b0* fn_710046eee4();

struct UnkDashSlash_l4687b0 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    f32 start_x;  // 0x30
    u8 pad_0x34[0xc];
    f32 distance;  // 0x40, how far the owner moves before the request goes out
    bool sent;     // 0x44
};

// A movement component: the point it heads for is at +0x17a0.
struct UnkMovement_l4687b0 {
    u8 pad_0x0[0x17a0];
    u8 target[8];
};

static inline UnkVec3f Pos_l4687b0(UnkActor* actor) {
    return UnkVec3f(actor->field_0x44, actor->field_0x54, actor->field_0x64);
}

static inline f32 DistanceSq_l4687b0(const UnkVec3f& a, const UnkVec3f& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}

static inline f32 Sign_l4687b0(f32 v) { return static_cast<f32>((v > 0.0f) - (v < 0.0f)); }

// Sends the request (mode 0) to the character class.
inline bool Send_l4687b0(void* target) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, 0xfb7d4bc21bc9379a, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                         &g_unk_7172769740, &mask);
}

void fn_71004687b0(UnkDashSlash_l4687b0* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    if (self->sent)
        return;
    UnkActor* owner = self->owner;
    if (owner->field_0x220 && !(__builtin_fabsf(owner->field_0x44 - self->start_x) < self->distance)) {
        if (fn_7101178340(static_cast<UnkAnimationComponent*>(owner->field_0x220), 0xf4061ff25a2163d6))
            self->sent = Send_l4687b0(self->owner->field_0x220);
        if (self->sent)
            return;
    }
    UnkEntity_l4687b0* own = static_cast<UnkEntity_l4687b0*>(self->owner->field_0x240);
    if (!own)
        return;
    UnkEntityList_l4687b0* list = fn_710046eee4();
    for (u32 i = 0; i < list->count; ++i) {
        UnkEntity_l4687b0* entity = Get_l4687b0(list->data[i]);
        if (entity == own)
            continue;
        UnkActor* other = entity->actor;
        if (!other)
            continue;
        UnkActor* me = self->owner;
        UnkVec3f pos;
        pos = Pos_l4687b0(me);
        UnkVec3f other_pos = Pos_l4687b0(other);
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(me->field_0x38));
        // The side the owner faces, and whether the other actor is on it and near.
        f32 facing = Sign_l4687b0(m.m[2]);
        bool near = facing == Sign_l4687b0(other_pos.x - pos.x) && DistanceSq_l4687b0(pos, other_pos) < 160000.0f;
        if (!near) {
            UnkMovement_l4687b0* movement = reinterpret_cast<UnkMovement_l4687b0*>(
                base::reflection::refl_FindTyped<UnkRefl_fn_7100468bb8>(other));
            if (!movement || fn_71010cc3a4(movement) != 4)
                continue;
            UnkVec3f p = fn_7100973658(reinterpret_cast<UnkUnit1444_o0309*>(movement->target));
            if (!(facing == Sign_l4687b0(p.x - pos.x) && DistanceSq_l4687b0(pos, p) < 160000.0f))
                continue;
        }
        self->sent = Send_l4687b0(self->owner->field_0x220);
    }
}
