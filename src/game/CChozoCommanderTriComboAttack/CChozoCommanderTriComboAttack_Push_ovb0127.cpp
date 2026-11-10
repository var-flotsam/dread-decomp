// CChozoCommanderTriComboAttack's unit (tools/tu_map.py): when the target stands on the side of the
// owner's collider (g_unk_71726c8520 names it) that the owner faces, marks the result (+0x49) and fills
// in the push: the facing direction (from the owner's matrix, fn_710007b444) and fixed scales. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkPlayerMovementSource;
void* fn_710086406c(UnkPlayerMovementSource*, const void*);
struct UnkVec3;
void fn_710007b444(void*, const UnkVec3*);

extern const u8 g_unk_71726c8520[] __attribute__((visibility("hidden")));  // the collider's name (a CStrId)

// A collider: its bounds' x extent at +0x40 and +0x48, refreshed (vf_0x98) while dirty (+0x60).
struct UnkTriComboCollider_ovb0127 {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 min_x;  // 0x40
    u8 pad_0x44[0x4];
    f32 max_x;  // 0x48
    u8 pad_0x4c[0x14];
    bool dirty;  // 0x60
    void vf_0x98() { reinterpret_cast<void (*)(UnkTriComboCollider_ovb0127*)>(vtable[0x98 / 8])(this); }
};

struct UnkTriComboOwner_ovb0127 {
    u8 pad_0x0[0x2a0];
    UnkPlayerMovementSource* colliders;  // 0x2a0
};

struct UnkTriComboResult_ovb0127 {
    u8 pad_0x0[0x49];
    bool hit;  // 0x49
    u8 pad_0x4a[0x2];
    u8 direction[0x4];  // 0x4c
    f32 scale_a;        // 0x50
    f32 scale_b;        // 0x54
    f32 scale_c;        // 0x58
};

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

static inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

void fn_710043bc98(CAttack* self, void* arg, UnkTriComboResult_ovb0127* result) {
    UnkPlayerMovementSource* colliders =
        reinterpret_cast<UnkTriComboOwner_ovb0127*>(self->field_0x8)->colliders;
    auto* collider = colliders ? static_cast<UnkTriComboCollider_ovb0127*>(fn_710086406c(colliders, g_unk_71726c8520))
                               : nullptr;
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    if (!collider || !target)
        return;
    if (collider->dirty)
        collider->vf_0x98();
    f32 side = Sign(target->field_0x44 + (collider->min_x + collider->max_x) * -0.5f);
    if (Sign(Matrix(self->field_0x8).m[2]) != side)
        return;
    result->hit = true;
    const UnkMatrix44& m = Matrix(self->field_0x8);
    f32 x = m.m[2];
    f32 y = m.m[6];
    f32 z = m.m[10];
    base::math::CVector3D forward;
    forward.x = x;
    forward.y = y;
    forward.z = z;
    fn_710007b444(result->direction, reinterpret_cast<const UnkVec3*>(&forward));
    result->scale_a = 1.0f;
    result->scale_b = 1.2f;
    result->scale_c = 0.5f;
}
