// Unit 2702 (CAIComponent's neighbourhood): an AI's bounding box around its owner: the owner's
// position (moved to the bone the bounding-box track names, when the animation plays one), its
// height offset, the box grown to the owner's half extents plus 100 units, then handed to the
// owner's bounds listener. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkVec3_l1061d0 {
    f32 x, y, z;
};

struct UnkBox_l1061d0 {
    UnkVec3_l1061d0 min;
    UnkVec3_l1061d0 max;
};

static inline f32 Min(f32 a, f32 b) { return a < b ? a : b; }
static inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }

struct UnkBoundsListener_l1061d0;
struct UnkBoundsListenerVtbl_l1061d0 {
    void* pad_0x0[26];
    void (*vf_0xd0)(UnkBoundsListener_l1061d0* self, UnkBox_l1061d0* box);
};
struct UnkBoundsListener_l1061d0 {
    UnkBoundsListenerVtbl_l1061d0* vtable;
};

struct UnkBoundsLink_l1061d0 {
    u8 pad_0x0[0x1a0];
    UnkBoundsListener_l1061d0* listener;
};

struct UnkBoundsOwner_l1061d0 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x1b8];
    UnkAnimationComponent* animation;  // 0x220
    void* field_0x228;
    u8 pad_0x230[0x48];
    UnkBoundsLink_l1061d0* field_0x278;
    u8 pad_0x280[0x108];
    f32 extent_x;  // 0x388
    f32 extent_y;
    f32 extent_z;
    f32 height;  // 0x394
};
static_assert(__builtin_offsetof(UnkBoundsOwner_l1061d0, height) == 0x394, "layout");

struct UnkBoundsSelf_l1061d0 {
    u8 pad_0x0[0x30];
    UnkBoundsOwner_l1061d0* owner;
};

struct UnkBoneRecord_l1061d0 {
    u8 pad_0x0[0x18];
    u64 id;
};

void fn_71011061d0(UnkBoundsComponent* component, UnkAabb* aabb) {
    auto* self = reinterpret_cast<UnkBoundsSelf_l1061d0*>(component);
    auto* out = reinterpret_cast<UnkBox_l1061d0*>(aabb);
    out->min.x = 0.0f;
    out->min.y = 0.0f;
    out->min.z = 0.0f;
    out->max.x = 0.0f;
    out->max.y = 0.0f;
    out->max.z = 0.0f;
    UnkBoundsOwner_l1061d0* owner = self->owner;
    if (!owner)
        return;
    alignas(8) UnkVec3_l1061d0 pos = {owner->x, owner->y, owner->z};
    if (UnkAnimationComponent* animation = owner->animation) {
        UnkTrackQuery query = {fn_7100c47708(), nullptr, -1.0f};
        fn_710117b08c(animation, &query);
        if (query.result)
            fn_710117364c(self->owner->field_0x228, &reinterpret_cast<UnkBoneRecord_l1061d0*>(query.result)->id,
                          reinterpret_cast<UnkGrappleAnswer_b50*>(&pos));
    }
    pos.y += self->owner->height;
    out->min = UnkVec3_l1061d0{pos.x + out->min.x, pos.y + out->min.y, pos.z + out->min.z};
    out->max = UnkVec3_l1061d0{pos.x + out->max.x, pos.y + out->max.y, pos.z + out->max.z};

    owner = self->owner;
    UnkVec3_l1061d0 half = {owner->extent_x * 0.5f, owner->extent_y * 0.5f, owner->extent_z * 0.5f};
    UnkVec3_l1061d0 lo = {pos.x - half.x - 100.0f, pos.y - half.y - 100.0f, pos.z - half.z - 100.0f};
    UnkVec3_l1061d0 hi = {pos.x + half.x + 100.0f, pos.y + half.y + 100.0f, pos.z + half.z + 100.0f};
    out->min = UnkVec3_l1061d0{Min(out->min.x, lo.x), Min(out->min.y, lo.y), Min(out->min.z, lo.z)};
    out->max = UnkVec3_l1061d0{Max(out->max.x, hi.x), Max(out->max.y, hi.y), Max(out->max.z, hi.z)};

    if (UnkBoundsLink_l1061d0* link = self->owner->field_0x278) {
        if (UnkBoundsListener_l1061d0* listener = link->listener)
            listener->vtable->vf_0xd0(listener, out);
    }
}
