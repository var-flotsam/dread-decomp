// CDredhedAttackComponent's unit (tools/tu_map.py unit 776): the hit handler. When the hit's collider is a
// CCollisionComponent whose owner is floor-sliding (and not vulnerable to it) or morphing (+0x210 or the
// converting track) with its movement's +0xf9 clear, and the attacker's animation does not allow flag 0x200
// (fn_7101178aec), the hit is ignored; otherwise the base handler (fn_7101137c8c) takes it. Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkEmmyAttackComponent_A3_b64;
struct UnkTileHit_A3_b64;
bool fn_7101137c8c(UnkEmmyAttackComponent_A3_b64* self, UnkTileHit_A3_b64* hit);
bool fn_7101178aec(UnkAnimationComponent* self, const u32* flags);

struct UnkDredhedMovement_l4e8374 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkDredhedTarget_l4e8374 {
    u8 pad_0x0[0x1f0];
    UnkDredhedMovement_l4e8374* movement;  // 0x1f0
    u8 pad_0x1f8[0x18];
    void* field_0x210;
    u8 pad_0x218[0x8];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkDredhedCollision_l4e8374 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkDredhedTarget_l4e8374* owner;  // 0x30
};

struct UnkDredhedCollider_l4e8374 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkDredhedHitInfo_l4e8374 {
    u8 pad_0x0[0x10];
    UnkDredhedCollider_l4e8374* collider;  // 0x10
};

struct UnkDredhedHit_l4e8374 {
    u8 pad_0x0[0x10];
    UnkDredhedHitInfo_l4e8374* info;  // 0x10
};

struct UnkDredhedAttack_l4e8374 {
    u8 pad_0x0[0x30];
    UnkDredhedTarget_l4e8374* owner;  // 0x30
};

// The base attack component's hit handler.
static inline u8 Base(UnkDredhedAttack_l4e8374* self, UnkDredhedHit_l4e8374* hit) {
    return fn_7101137c8c(reinterpret_cast<UnkEmmyAttackComponent_A3_b64*>(self),
                         reinterpret_cast<UnkTileHit_A3_b64*>(hit));
}

u8 fn_71004e8374(UnkDredhedAttack_l4e8374* self, UnkDredhedHit_l4e8374* hit) {
    UnkReflObject* component = hit->info->collider->component;
    if (component && fn_710009d50c(component->vf_0x0(), fn_710085fc44())) {
        UnkDredhedTarget_l4e8374* target = static_cast<UnkDredhedCollision_l4e8374*>(component)->owner;
        UnkAnimationComponent* animation = target->animation;
        UnkAnimationComponent* own = self->owner->animation;
        bool sliding = fn_710117b348(animation, fn_7100cf919c()) && !fn_710117b348(animation, fn_7100c64f08());
        bool morphing = (target->field_0x210 || fn_710117b348(animation, fn_7100cf2ae8())) &&
                        !target->movement->field_0xf9;
        if (animation && own) {
            alignas(8) u32 flags = 0x200;
            bool allowed = fn_7101178aec(own, &flags);
            if ((sliding || morphing) && !allowed)
                return 0;
        }
    }
    return Base(self, hit);
}
