// CGooplotAIComponent (tools/tu_map.py unit 851): the per-frame update. After the base update it
// keeps the movement's last non-zero velocity, resets the AI when the owner leaves its area, and
// runs the timer at +0x14898 against the state's limit. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CAttackComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CGooplotAIComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkAIGroup_g;
struct UnkQuetzoaArea_A3_b75;
UnkAIGroup_g* fn_71011277a4(void* src);
UnkQuetzoaArea_A3_b75* fn_7101127964(void* spawn_point);
UnkQuetzoaArea_A3_b75* fn_7101119c90(UnkAIGroup_g* group);
bool fn_710059ad88();

struct UnkGooplotArea_l59ab2c {
    void** vtable;

    bool Contains(const CVector2D* point) {
        return reinterpret_cast<bool (*)(UnkGooplotArea_l59ab2c*, const CVector2D*)>(vtable[0x50 / 8])(this, point);
    }
};

struct UnkGooplotMovement_l59ab2c {
    void** vtable;
    u8 pad_0x8[0x68];
    UnkVec3f last_velocity;  // 0x70

    UnkVec3f GetVelocity() {
        return reinterpret_cast<UnkVec3f (*)(UnkGooplotMovement_l59ab2c*)>(vtable[0x370 / 8])(this);
    }
};

struct UnkGooplotSpawn_l59ab2c {
    u8 pad_0x0[0x10];
    void* spawn_point;  // 0x10
};

struct UnkGooplotOwner_l59ab2c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x218 - 0x94];
    UnkGooplotMovement_l59ab2c* movement;  // 0x218
    u8 pad_0x220[0x58];
    void* attack;  // 0x278
    u8 pad_0x280[0x118];
    UnkGooplotSpawn_l59ab2c* spawn;  // 0x398
};

struct UnkGooplotState_l59ab2c {
    u8 pad_0x0[0x6f0];
    u32 mode;   // 0x6f0
    f32 limit;  // 0x6f4
};

struct UnkGooplotAI_l59ab2c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkGooplotOwner_l59ab2c* owner;  // 0x30
    u8 pad_0x38[0x146d8 - 0x38];
    bool active;  // 0x146d8
    u8 pad_0x146d9[0x147f0 - 0x146d9];
    UnkGooplotState_l59ab2c* state;  // 0x147f0
    u32 field_0x147f8;
    u8 pad_0x147fc[0x14873 - 0x147fc];
    bool field_0x14873;
    u8 pad_0x14874[0x14898 - 0x14874];
    f32 timer;  // 0x14898

    void vf_0x7b8() { reinterpret_cast<void (*)(UnkGooplotAI_l59ab2c*)>(vtable[0x7b8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkGooplotAI_l59ab2c, timer) == 0x14898, "timer");

static inline bool IsNearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

// Whether the owner stands outside its spawn point's area (or its group's, when the spawn point has
// none); false when there is no area at all.
static inline bool IsOutsideArea(UnkGooplotAI_l59ab2c* self) {
    UnkGooplotSpawn_l59ab2c* spawn = self->owner->spawn;
    void* spawn_point = spawn ? spawn->spawn_point : nullptr;
    UnkAIGroup_g* group = fn_71011277a4(spawn_point);
    UnkQuetzoaArea_A3_b75* area = fn_7101127964(spawn_point);
    if (!area) {
        area = fn_7101119c90(group);
        if (!area)
            return false;
    }
    UnkVec3f pos = self->owner->xform.GetTranslation();
    CVector2D pos2;
    fn_710007b430(&pos2, reinterpret_cast<const CVector3D*>(&pos));
    return !reinterpret_cast<UnkGooplotArea_l59ab2c*>(area)->Contains(&pos2);
}

void fn_710059ab2c(UnkGooplotAI_l59ab2c* self, f32 dt) {
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    if (self->field_0x14873)
        self->vf_0x7b8();

    {
        UnkGooplotMovement_l59ab2c* movement = self->owner->movement;
        UnkVec3f velocity = movement->GetVelocity();
        if (!(IsNearZero(velocity.x) && IsNearZero(velocity.y) && IsNearZero(velocity.z)))
            movement->last_velocity = velocity;
    }

    if (IsOutsideArea(self)) {
        self->field_0x147f8 = 0;
        UnkGooplotOwner_l59ab2c* owner = self->owner;
        if (owner && owner->attack) {
            void* owner_movement = owner->movement;
            bool attacking = fn_7101136308(owner->attack);
            if (owner_movement && !attacking && fn_71010cc3a4(owner_movement) != 3) {
                if (self->state->mode == 0 && fn_710059ad88())
                    self->active = true;
                else
                    fn_710059aa2c(reinterpret_cast<UnkGooplotAI_b55*>(self));
            }
        }
    }

    if (self->active) {
        self->timer += dt;
        if (self->timer >= self->state->limit)
            fn_710059aa2c(reinterpret_cast<UnkGooplotAI_b55*>(self));
    }
}
