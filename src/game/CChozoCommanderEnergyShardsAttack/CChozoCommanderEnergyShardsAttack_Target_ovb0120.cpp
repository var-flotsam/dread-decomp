// CChozoCommanderEnergyShardsAttack's unit (tools/tu_map.py unit 647): picks where the shards go
// (fn_710042b1a0). With the owner's AI a CChozoCommanderAIComponent, its range (+0x15d1c, ends at +0 and
// +8) gives a middle; an owner past the middle and facing outwards (the z of the matrix's first row)
// targets halfway to that side's end, at the owner's y; otherwise the ground attack's choice
// (fn_710042bb48) decides. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSelf;
struct UnkOut;
bool fn_710042bb48(UnkSelf* self, UnkOut* out);

struct UnkShardsRange_ovb0120 {
    f32 left;
    f32 pad_0x4;
    f32 right;
};

struct UnkShardsAI_ovb0120 {
    u8 pad_0x0[0x15d1c];
    UnkShardsRange_ovb0120 range;  // 0x15d1c
};

struct UnkShardsOwner_ovb0120 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38 (its translation x at +0x44)
    u8 pad_0x78[0x1c8];
    UnkReflObject* ai;  // 0x240
};
static_assert(__builtin_offsetof(UnkShardsOwner_ovb0120, ai) == 0x240, "layout");

struct UnkShardsAttack_ovb0120 {
    u8 pad_0x0[0x8];
    UnkShardsOwner_ovb0120* owner;  // 0x8
};

struct UnkShardsTarget_ovb0120 {
    f32 x;
    u32 y;
};

static inline f32 PosX(const UnkShardsOwner_ovb0120* owner) {
    return reinterpret_cast<const f32*>(owner)[0x44 / 4];
}
static inline u32 PosY(const UnkShardsOwner_ovb0120* owner) {
    return reinterpret_cast<const u32*>(owner)[0x54 / 4];
}

bool fn_710042b1a0(UnkShardsAttack_ovb0120* self, UnkShardsTarget_ovb0120* out) {
    UnkReflObject* ai = self->owner->ai;
    bool isCommander = fn_710009d50c(ai->vf_0x0(), fn_710044e708());
    UnkShardsRange_ovb0120* range = &reinterpret_cast<UnkShardsAI_ovb0120*>(isCommander ? ai : nullptr)->range;
    UnkShardsOwner_ovb0120* owner = self->owner;
    f32 mid = (range->left + range->right) * 0.5f;
    f32 x = PosX(owner);
    f32 facing = fn_710002614c(&owner->xform).m[2];
    if (x > mid && facing > 0.0f) {
        out->y = PosY(self->owner);
        out->x = (mid + range->left) * 0.5f;
        return true;
    }
    if (x < mid && facing < 0.0f) {
        out->y = PosY(self->owner);
        out->x = (mid + range->right) * 0.5f;
        return true;
    }
    return fn_710042bb48(reinterpret_cast<UnkSelf*>(self), reinterpret_cast<UnkOut*>(out));
}
