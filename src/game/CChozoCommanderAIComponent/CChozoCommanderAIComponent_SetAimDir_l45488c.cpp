// CChozoCommanderAIComponent unit (tools/tu_map.py unit 665): stores the normalized aim direction at
// +0x15ed4, then, when the owner has a value bag, reports the direction's angle in the owner's local
// frame, in degrees, under "Angle". Names and layouts are placeholders.
#include <math.h>
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkMatrix44Words {
    u32 m[16];

    UnkMatrix44Words() {}
    UnkMatrix44Words(const UnkMatrix44Words& other);
};

UnkMatrix44Words fn_7100076dc4(const UnkMatrix44Words* src);
UnkMatrix44 fn_7100078314(const UnkMatrix44* m);

struct UnkCommanderBagOwner_l45488c {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag bag;  // 0x80
};

struct UnkCommanderOwner_l45488c {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1a8];
    UnkCommanderBagOwner_l45488c* bag_owner;  // 0x220
};
static_assert(__builtin_offsetof(UnkCommanderOwner_l45488c, bag_owner) == 0x220, "layout");

struct UnkCommanderAI_l45488c {
    u8 pad_0x0[0x30];
    UnkCommanderOwner_l45488c* owner;  // 0x30
    u8 pad_0x38[0x15e9c];
    UnkVec3f aim_dir;  // 0x15ed4
};
static_assert(__builtin_offsetof(UnkCommanderAI_l45488c, aim_dir) == 0x15ed4, "layout");

// The address of a temporary, for the callees that take their matrix by pointer.
template <class T>
static inline const T* AddressOf(const T& value) {
    return &value;
}

void fn_710045488c(UnkCommanderAI_l45488c* self, const UnkVec3f* dir) {
    UnkVec3f* aim = &self->aim_dir;
    f32 inv = 1.0f / (sqrtf(dir->x * dir->x + dir->y * dir->y + dir->z * dir->z) + 0.0001f);
    f32 x = dir->x * inv;
    f32 y = dir->y * inv;
    f32 z = dir->z * inv;
    aim->x = x;
    aim->y = y;
    aim->z = z;

    UnkCommanderOwner_l45488c* owner = self->owner;
    UnkCommanderBagOwner_l45488c* bag_owner = owner->bag_owner;
    if (!bag_owner)
        return;
    // The aim direction in the owner's frame: through the inverse of the owner's rotation.
    UnkVec3f local = fn_7100078170(
        reinterpret_cast<const UnkMat4f*>(AddressOf(fn_7100078314(reinterpret_cast<const UnkMatrix44*>(
            AddressOf(fn_7100076dc4(reinterpret_cast<const UnkMatrix44Words*>(&fn_710002614c(&owner->transform)))))))),
        aim);
    f32 angle = atan2f(local.y, local.z) * 180.0f / 3.14159265358979323846;
    unk_prop_bag::fn_710033187c(&bag_owner->bag, CStrId("Angle", true), angle);
}
