// CQuetzoaXMultiTargetAttack's unit (tools/tu_map.py unit 960): launches one multi-target projectile. It
// spawns the projectile's actor definition, aims it from the launch node towards the target, turned by the
// projectile's spread angle (alternating sides), and starts its movement. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTU54.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkEntity;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;

extern char g_unk_710164a390[];                                          // the launch node's key
__attribute__((visibility("hidden"))) extern const u8 g_unk_710164a398[];  // the basis' extra argument
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726fa878;      // the projectile's definition

void* fn_710063df28(void* owner);  // refl_FindTyped of "MOVEMENT".
UnkVec3f fn_710007a390(const UnkVec3f& v, f32 angle);
void fn_71000769f0(UnkMatrix44* out, const void* a, const void* b, const void* c, const void* d);

struct UnkActorDefName_l63d920 {
    u8 pad_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct alignas(8) UnkActorDefType_l63d920 {
    u32 id;
};

// The projectile's class: its spread angle in degrees.
struct UnkProjectileClass_l63d920 : UnkReflObject {
    u8 pad_0x8[0x138];
    f32 spread;  // 0x140
};

struct UnkProjectileMovement_l63d920 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x28];
    UnkVec3f direction;  // 0x40
    u8 pad_0x4c[0x598];
    UnkVec3f target;  // 0x5e4

    void vf_0x290(UnkActor* owner) {
        reinterpret_cast<void (*)(UnkProjectileMovement_l63d920*, UnkActor*)>(vtable[0x290 / 8])(this, owner);
    }
    void vf_0x2c0() { reinterpret_cast<void (*)(UnkProjectileMovement_l63d920*)>(vtable[0x2c0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkProjectileMovement_l63d920, target) == 0x5e4, "layout");

struct UnkQuetzoaXMultiTargetAttack_l63d920 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x2c];
    f32 side;  // 0x3c: the spread's sign, flipped on every launch
};

// vtable slot 0xd8: sets the actor's rotation angles.
inline void SetAngles(UnkActor* actor, const UnkVec3f* angles, bool flag) {
    reinterpret_cast<void (*)(UnkActor*, const UnkVec3f*, bool)>(actor->vtable[0xd8 / 8])(actor, angles, flag);
}

inline f32 SignOf(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

inline f32 DegToRad(f32 deg) {
    return deg * 3.141592653589793 / 180.0;
}

void fn_710063d920(UnkQuetzoaXMultiTargetAttack_l63d920* self) {
    UnkActor* owner = self->owner;
    UnkVec3f origin(owner->field_0x44, owner->field_0x54, 0.0f);
    UnkMatrix44 node = {};
    node.m[15] = 1.0f;
    node.m[10] = 1.0f;
    node.m[5] = 1.0f;
    node.m[0] = 1.0f;
    if (fn_71011735e8(static_cast<UnkEntity*>(owner->field_0x228), g_unk_710164a390, &node, 0)) {
        const UnkVec3f offset(0.0f, 0.0f, 0.0f);
        origin = UnkVec3f(node.m[3], node.m[7], 0.0f) + offset;
    }

    UnkActor* target = fn_7100d7c990(self->owner);
    UnkVec3f target_pos;
    target_pos = reinterpret_cast<const UnkXform*>(target->field_0x38)->GetTranslation();
    UnkVec3f dir = target_pos - origin;
    const UnkMatrix44& facing = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    if (SignOf(facing.m[2]) != SignOf(dir.x)) {
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        dir = UnkVec3f(m.m[2], m.m[6], m.m[10]);
    }
    dir.z = 0.0f;

    UnkActor* projectile;
    {
        UnkActorDefType_l63d920 type;
        UnkActorDefFactory* factory = g_unk_71726a0220;
        UnkActorDefName_l63d920* name = reinterpret_cast<UnkActorDefName_l63d920*>(g_unk_71726fa878.data);
        ++name->use_count;
        const char* text = reinterpret_cast<UnkActorDefName_l63d920*>(g_unk_71726fa878.data)->text;
        type.id = g_unk_7172a0ba48;
        projectile = reinterpret_cast<UnkActor*>(fn_710119438c(
            factory, text, &g_unk_71726fa878, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    }
    if (!projectile)
        return;
    UnkProjectileMovement_l63d920* movement =
        static_cast<UnkProjectileMovement_l63d920*>(fn_710063df28(projectile));
    if (!movement)
        return;

    UnkReflObject* char_class = movement->char_class;
    UnkProjectileClass_l63d920* cls =
        fn_710009d50c(char_class->vf_0x0(), fn_7100640b04()) ? static_cast<UnkProjectileClass_l63d920*>(char_class)
                                                              : nullptr;
    f32 spread = DegToRad(cls->spread);
    UnkVec3f aim = fn_710007a390(dir, self->side * spread);
    aim = Normalized(aim);
    self->side = -self->side;

    UnkVec3f pos = origin + aim * 50.0f;
    projectile->vf_0xd0(&pos, true);

    UnkVec3f right = Normalized(Cross(UnkVec3f(0.0f, 1.0f, 0.0f), aim));
    UnkVec3f up = Normalized(Cross(aim, right));
    UnkMatrix44 basis;
    fn_71000769f0(&basis, &right, &up, &aim, g_unk_710164a398);
    UnkVec3f angles = fn_7100076acc(reinterpret_cast<const UnkXform*>(&basis));
    SetAngles(projectile, &angles, true);

    movement->direction = aim;
    movement->vf_0x290(self->owner);
    movement->target = target_pos;
    movement->vf_0x2c0();
}
