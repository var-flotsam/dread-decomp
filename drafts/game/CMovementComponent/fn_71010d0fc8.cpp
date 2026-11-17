// CMovementComponent (unit 2688): the point the component would stand on below `pos`, probed at most
// `depth` down, lifted by the shape's height. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkAssignedSlot;

// A reference to a counted object, assigned out of line (fn_71002a5d1c).
struct UnkSlot_ovb0048 {
    base::UnkRefCounted* ptr;

    UnkSlot_ovb0048() : ptr(nullptr) {}
    ~UnkSlot_ovb0048() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }

    UnkSlot_ovb0048& operator=(const UnkSlot_ovb0048& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, then the point, a
// second vector and the normal (+0x2c), and three words, all copied as words.
struct UnkFloorRef_ovb0048 {
    UnkSlot_ovb0048 slot_a;
    UnkSlot_ovb0048 slot_b;
    u32 field_0x10[13];

    UnkFloorRef_ovb0048() {
        field_0x10[0] = 0xffffffff;
        for (u32 i = 1; i < 13; i++)
            field_0x10[i] = 0;
    }
    const UnkVec3f& Point() const { return *reinterpret_cast<const UnkVec3f*>(&field_0x10[1]); }

    UnkFloorRef_ovb0048& operator=(const UnkFloorRef_ovb0048& other) {
        if (this != &other) {
            slot_a = other.slot_a;
            slot_b = other.slot_b;
            for (u32 i = 0; i < 13; i++)
                field_0x10[i] = other.field_0x10[i];
        }
        return *this;
    }
};

struct UnkProbeShape_ovb0048 {
    u8 pad_0x0[0x60];
    f32 height;
};

struct UnkProbeOwner_ovb0048 {
    u8 pad_0x0[0x228];
    UnkProbeShape_ovb0048* shape;  // 0x228
    u8 pad_0x230[0x378 - 0x230];
    u32 collision_mask;  // 0x378
};

struct UnkProbeMovement_ovb0048 {
    u8 pad_0x0[0x30];
    UnkProbeOwner_ovb0048* owner;  // 0x30
};

// Without a shape, `pos` itself. Otherwise the floor point found below `pos` (or `pos` lowered by
// `depth` when there is none), raised by the shape's height; `found` and `out_hit` take the probe's result.
UnkVec3f fn_71010d0fc8(UnkProbeMovement_ovb0048* self, f32 depth, const UnkVec3f* pos, bool* found,
                       UnkFloorRef_ovb0048* out_hit) {
    UnkProbeOwner_ovb0048* owner = self->owner;
    if (!owner->shape)
        return *pos;
    void* tree = g_unk_7101d47310->field_0x608->field_0x820;
    UnkFloorRef_ovb0048 hit;
    u32 mask = owner->collision_mask;
    UnkVec3f end(pos->x, pos->y - depth, pos->z);
    if (fn_7100d5b714(tree, mask, pos, &end, &hit, true, false, 0.1f)) {
        if (found)
            *found = true;
        if (out_hit)
            *out_hit = hit;
        const UnkVec3f& point = hit.Point();
        return UnkVec3f(point.x + 0.0f, self->owner->shape->height + point.y, point.z + 0.0f);
    }
    if (found)
        *found = false;
    return UnkVec3f(pos->x + 0.0f, (self->owner->shape->height - depth) + pos->y, pos->z + 0.0f);
}
