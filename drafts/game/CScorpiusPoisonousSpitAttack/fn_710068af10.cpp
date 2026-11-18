// CScorpiusPoisonousSpitAttack's unit (tools/tu_map.py unit 1021): spits a poison projectile. Loads the
// projectile's actor definition, places it at the owner's spit node (else the owner's position), and gives
// its movement component a direction 45 degrees up, facing the owner's way, and the launch speed that
// reaches the target (or the player). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkXformNode.h"
#include "types.h"

#include <math.h>

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;
struct UnkEntity;
struct UnkHelper;

// The projectile's actor definition.
extern CStrId g_unk_7172703620 __attribute__((visibility("hidden")));
// The spit node of the owner's model.
extern char g_unk_710164a530[] __attribute__((visibility("hidden")));

char* fn_71011729a8(char* p0);
UnkVec3f fn_710007a390(const UnkVec3f& v, f32 angle);

// A position returned in memory (x8): 16 bytes, not trivially copyable.
struct UnkActorPosition {
    f32 x, y, z, w;
    UnkActorPosition(const UnkActorPosition& other);
};

struct UnkRefl_fn_710068b1ec;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_710068b1ec>(void*);

struct UnkSpitName_l68af10 {
    u8 unk_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct alignas(8) UnkSpitDefType_l68af10 {
    u32 id;
};

struct UnkSpitCharClass_l68af10 : UnkReflObject {
    u8 pad_0x8[0xcc];
    f32 field_0xd4;
};

struct UnkSpitMovement_l68af10 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkSpitCharClass_l68af10* char_class;  // 0x10
    u8 pad_0x18[0x28];
    UnkVec3f direction;  // 0x40
    u8 pad_0x4c[0xbc];
    f32 speed;  // 0x108

    void vf_0x290(UnkActor* owner) {
        reinterpret_cast<void (*)(UnkSpitMovement_l68af10*, UnkActor*)>(vtable[0x290 / 8])(this, owner);
    }
    void vf_0x2c0() { reinterpret_cast<void (*)(UnkSpitMovement_l68af10*)>(vtable[0x2c0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkSpitMovement_l68af10, speed) == 0x108, "layout");

struct UnkSpitAttack_l68af10 {
    void* vtable;
    UnkActor* owner;  // 0x8
};

void fn_710068af10(UnkSpitAttack_l68af10* self) {
    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkSpitName_l68af10* name = reinterpret_cast<UnkSpitName_l68af10*>(g_unk_7172703620.data);
    ++name->use_count;
    UnkSpitDefType_l68af10 type;
    type.id = g_unk_7172a0ba48;
    UnkActor* spit = reinterpret_cast<UnkActor*>(fn_710119438c(
        factory, name->text, &g_unk_7172703620, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    if (!spit)
        return;

    // Where it starts: the spit node when the model has one, else the owner's position.
    UnkActor* owner = self->owner;
    UnkVec3f position(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    UnkMatrix44 node = {};
    node.m[15] = 1.0f;
    node.m[10] = 1.0f;
    node.m[5] = 1.0f;
    node.m[0] = 1.0f;
    fn_71011729a8(static_cast<char*>(owner->field_0x228));
    if (fn_71011735e8(static_cast<UnkEntity*>(self->owner->field_0x228), g_unk_710164a530, &node, 0))
        position = UnkVec3f(node.m[3], node.m[7], node.m[11]);

    // 45 degrees up, facing the owner's way.
    UnkVec3f direction = fn_710007a390(UnkVec3f(1.0f, 0.0f, 0.0f), 0.7853982f);
    if (fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38)).m[2] < 0.0f)
        direction.x = -direction.x;
    f32 length = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    f32 inverse = 1.0f / (length + 0.0001f);
    direction.x = direction.x * inverse;
    direction.y = inverse * direction.y;
    direction.z = inverse * direction.z;

    spit->vf_0xd0(&position, true);
    UnkSpitMovement_l68af10* movement = reinterpret_cast<UnkSpitMovement_l68af10*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_710068b1ec>(spit));
    if (!movement)
        return;
    UnkSpitCharClass_l68af10* charClass = movement->char_class;
    if (!charClass || !fn_710009d50c(charClass->vf_0x0(), fn_7100698988()))
        return;
    movement->direction = direction;
    movement->vf_0x290(self->owner);

    // The speed that lands it on the target at 45 degrees.
    UnkActor* target = fn_7100d7c990(self->owner);
    UnkActorPosition aim = fn_7100d7b7a8(target);
    if (target && target->field_0x208 && fn_71009e6434(static_cast<UnkHelper*>(target->field_0x208))) {
        f32 x = target->field_0x44;
        f32 y = target->field_0x54;
        f32 z = target->field_0x64;
        aim.x = x;
        aim.y = y;
        aim.z = z;
    }
    const f32 cos45 = 0.70710677f;
    f32 dx = aim.x - position.x;
    f32 dy = aim.y - position.y;
    f32 rise = dy - fabsf(dx);
    f32 gravity = charClass->field_0xd4;
    f32 speed = rise * (gravity * (gravity * -2.0f) * cos45 * cos45) / (dx * dx);
    movement->speed = speed <= 100.0f ? 100.0f : (!(speed >= 7500.0f) ? speed : 7500.0f);
    movement->vf_0x2c0();
}
