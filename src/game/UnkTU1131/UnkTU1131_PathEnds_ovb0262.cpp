// Unit 1131: whether the two path sequences of the owner (+0x12ab8 and +0x25540 into its object at
// +0x18) both have a path, whose last nodes are more than 0.001 (squared distance) apart, and the node
// the second sequence reaches (from its last entry, three back when there are more than two) is a
// usable IPathNode: both links set and its flag at +0x64 true. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPathNode_ovb0262s {
    virtual const base::reflection::CClass* vf_0x0() const;
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkVec3f* vf_0x18();
    u8 pad_0x8[0x12];
    s16 field_0x1a;
    u8 pad_0x1c[0x1a];
    s16 field_0x36;
    u8 pad_0x38[0x2c];
    bool field_0x64;
};

struct UnkPathItem_ovb0262s {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 vf_0x28();
    virtual UnkPathNode_ovb0262s* vf_0x30(u32 index);
};

struct UnkPathSequence_ovb0262s {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    virtual u32 vf_0x28();
    virtual UnkPathItem_ovb0262s* vf_0x30(u32 index);
};

struct UnkPathUser_ovb0262s {
    u8 pad_0x0[0x18];
    u8* owner;  // 0x18
};

bool fn_71007080dc(void* self_) {
    u8* owner = static_cast<UnkPathUser_ovb0262s*>(self_)->owner;
    auto* first = reinterpret_cast<UnkPathSequence_ovb0262s*>(owner + 0x12ab8);
    if (!first->vf_0x20() || !first->vf_0x28())
        return false;
    auto* second = reinterpret_cast<UnkPathSequence_ovb0262s*>(owner + 0x25540);
    if (!second->vf_0x20() || !second->vf_0x28())
        return false;
    u32 first_count = reinterpret_cast<UnkPathItem_ovb0262s*>(first->vf_0x30(0))->vf_0x28();
    u32 second_count = reinterpret_cast<UnkPathItem_ovb0262s*>(second->vf_0x30(0))->vf_0x28();
    if (first_count == 0 || second_count == 0)
        return false;
    UnkPathNode_ovb0262s* node = second->vf_0x30(0)->vf_0x30(second_count > 2 ? second_count - 3 : 0);
    if (!node)
        return false;
    if (!fn_710009d50c(node->vf_0x0(), fn_7100da4088()))
        return false;
    if (node->field_0x1a == -1 || node->field_0x36 == -1 || !node->field_0x64)
        return false;
    const UnkVec3f* a = first->vf_0x30(0)->vf_0x30(first_count - 1)->vf_0x18();
    const UnkVec3f* b = second->vf_0x30(0)->vf_0x30(second_count - 1)->vf_0x18();
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 sum = dx * dx + dy * dy;
    f32 dz = a->z - b->z;
    return sum + dz * dz > 0.001f;
}
