// CKraidShockerSplashAttack's unit (tools/tu_map.py unit 917): spawns a shocker splash. Loads its actor
// definition, places it at the owner's splash node (else the owner's position), and gives its movement
// component a direction 45 degrees up, facing the owner's way, and the launch speed that reaches the
// player. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
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

// The splash's actor definition.
extern CStrId g_unk_71726f3018 __attribute__((visibility("hidden")));
// The id the player is looked up with.
extern CStrId g_unk_71726f3010 __attribute__((visibility("hidden")));
// The splash node of the owner's model.
extern char g_unk_710164a170[];

UnkVec3f RotateVec3AboutZ(const UnkVec3f& v, f32 angle);

struct UnkRefl_fn_71005f5f1c;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_71005f5f1c>(void*);

struct UnkSplashName_l5f5c3c {
    u8 unk_0x0[0x10];
    char text[0x20];
    u64 id;  // 0x30
    u8 unk_0x38[0xc];
    u32 use_count;  // 0x44
};

struct alignas(8) UnkSplashDefType_l5f5c3c {
    u32 id;
};

struct UnkSplashCharClass_l5f5c3c : UnkReflObject {
    u8 pad_0x8[0xcc];
    f32 field_0xd4;
};

struct UnkSplashMovement_l5f5c3c {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkSplashCharClass_l5f5c3c* char_class;  // 0x10
    u8 pad_0x18[0x28];
    UnkVec3f direction;  // 0x40
    u8 pad_0x4c[0x30];
    f32 speed;  // 0x7c

    void vf_0x290(UnkActor* owner) {
        reinterpret_cast<void (*)(UnkSplashMovement_l5f5c3c*, UnkActor*)>(vtable[0x290 / 8])(this, owner);
    }
    f32 vf_0x2d0() { return reinterpret_cast<f32 (*)(UnkSplashMovement_l5f5c3c*)>(vtable[0x2d0 / 8])(this); }
    void vf_0x2c0() { reinterpret_cast<void (*)(UnkSplashMovement_l5f5c3c*)>(vtable[0x2c0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkSplashMovement_l5f5c3c, speed) == 0x7c, "layout");

struct UnkSplashAttack_l5f5c3c {
    void* vtable;
    UnkActor* owner;  // 0x8
};

void fn_71005f5c3c(UnkSplashAttack_l5f5c3c* self) {
    alignas(8) u32 flags;  // the kind the player is looked up with
    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkSplashName_l5f5c3c* name = reinterpret_cast<UnkSplashName_l5f5c3c*>(g_unk_71726f3018.data);
    ++name->use_count;
    UnkSplashDefType_l5f5c3c type;
    type.id = g_unk_7172a0ba48;
    UnkActor* splash = reinterpret_cast<UnkActor*>(fn_710119438c(
        factory, name->text, &g_unk_71726f3018, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    if (!splash)
        return;

    // Where it starts: the splash node when the model has one, else the owner's position.
    UnkActor* owner = self->owner;
    UnkVec3f position(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    UnkMatrix44 node = {};
    node.m[15] = 1.0f;
    node.m[10] = 1.0f;
    node.m[5] = 1.0f;
    node.m[0] = 1.0f;
    if (fn_71011735e8(static_cast<UnkEntity*>(owner->field_0x228), g_unk_710164a170, &node, 0))
        position = UnkVec3f(node.m[3], node.m[7], node.m[11]);

    // 45 degrees up, facing the owner's way.
    UnkVec3f direction = RotateVec3AboutZ(UnkVec3f(1.0f, 0.0f, 0.0f), 0.7853982f);
    if (fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38)).m[2] < 0.0f)
        direction.x = -direction.x;
    f32 length = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    f32 inverse = 1.0f / (length + 0.0001f);
    direction.x = direction.x * inverse;
    direction.y = inverse * direction.y;
    direction.z = inverse * direction.z;

    splash->vf_0xd0(&position, true);
    UnkSplashMovement_l5f5c3c* movement = reinterpret_cast<UnkSplashMovement_l5f5c3c*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_71005f5f1c>(splash));
    if (!movement)
        return;
    UnkSplashCharClass_l5f5c3c* charClass = movement->char_class;
    if (!charClass || !fn_710009d50c(charClass->vf_0x0(), fn_71006035f4()))
        return;
    movement->direction = direction;
    movement->vf_0x290(self->owner);

    // The speed that lands it on the player at 45 degrees.
    UnkGameManager* manager = g_unk_7101d47310;
    u64 id;
    u32 flags_value = g_unk_71727bebb8;
    id = reinterpret_cast<UnkSplashName_l5f5c3c*>(g_unk_71726f3010.data)->id;
    flags = flags_value;
    UnkActor* player = static_cast<UnkActor*>(fn_7100deba8c(manager, &id, &flags));
    if (!player)
        return;
    const f32 cos45 = 0.70710677f;
    f32 targetX = player->field_0x44;
    f32 targetY = player->field_0x54;
    f32 dy = targetY - position.y;
    f32 dx = fabsf(targetX - position.x);
    f32 gravity = movement->vf_0x2d0();
    f32 denominator = (dy - dx) * -2.0f * cos45 * cos45;
    f32 speedSq = dx * (dx * gravity) / denominator;
    movement->speed = sqrtf(fabsf(speedSq)) / charClass->field_0xd4;
    movement->vf_0x2c0();
}
