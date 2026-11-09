// game::logic::collision::CColliderGroup (unit 2504 of tools/tu_map.py): the group's mask, rebuilt
// from its children (slot 0x58) and reported to the owner at +0x30 (fn_7100d5c888), and its bounds,
// grown over the enabled children that answer slot 0x58 (each refreshed through slot 0x98 first when
// its flag at +0x60 is set). The children are an inline list at +0x124 (a u32 count followed
// directly by the pointers). Names and layouts are placeholders.
#include "types.h"

struct UnkColliderNode_A6_b70;
void fn_7100d5c888(void* owner, UnkColliderNode_A6_b70* collider, u32 old_mask, u32 new_mask);
void fn_7100d35aec(void* self);

struct UnkBox_ovb0066 {
    f32 min_x, min_y;
    f32 max_x, max_y;

    UnkBox_ovb0066& operator=(const UnkBox_ovb0066& o) {
        min_x = o.min_x;
        min_y = o.min_y;
        max_x = o.max_x;
        max_y = o.max_y;
        return *this;
    }
};

struct UnkGroupChild_ovb0066 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkBox_ovb0066 bounds;  // 0x40
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
    u8 pad_0x61[0x9];
    bool enabled;  // 0x6a

    u32 vf_0x58() { return reinterpret_cast<u32 (*)(UnkGroupChild_ovb0066*)>(vtable[0x58 / 8])(this); }
    void vf_0x98() { reinterpret_cast<void (*)(UnkGroupChild_ovb0066*)>(vtable[0x98 / 8])(this); }
};

#pragma pack(push, 4)
struct UnkColliderGroup_ovb0066 {
    void** vtable;
    u8 pad_0x8[0x28];
    void* owner;  // 0x30
    u8 pad_0x38[0x4];
    bool keep_bounds;  // 0x3c
    u8 pad_0x3d[0x3];
    UnkBox_ovb0066 bounds;       // 0x40
    UnkBox_ovb0066 last_bounds;  // 0x50
    u8 pad_0x60[0x4];
    u32 mask;  // 0x64
    u8 pad_0x68[0x1];
    bool bounds_reset;  // 0x69
    u8 pad_0x6a[0x124 - 0x6a];
    u32 count;
    u8 children[8];  // 0x128, raw storage

    UnkGroupChild_ovb0066** begin() { return reinterpret_cast<UnkGroupChild_ovb0066**>(children); }
    UnkGroupChild_ovb0066** end() { return begin() + count; }
};
#pragma pack(pop)

void fn_7100d4598c(UnkColliderGroup_ovb0066* self) {
    u32 old_mask = self->mask;
    self->mask = 0;
    u32 mask = 0;
    for (UnkGroupChild_ovb0066** it = self->begin(); it != self->end(); ++it) {
        mask |= (*it)->vf_0x58();
        self->mask = mask;
    }
    if (self->owner)
        fn_7100d5c888(self->owner, reinterpret_cast<UnkColliderNode_A6_b70*>(self), old_mask, mask);
}

void fn_7100d45af4(UnkColliderGroup_ovb0066* self) {
    fn_7100d35aec(self);
    self->last_bounds = self->bounds;
    if (!self->keep_bounds) {
        self->bounds.min_x = 3.4028235e38f;
        self->bounds.min_y = 3.4028235e38f;
        self->bounds.max_x = -3.4028235e38f;
        self->bounds.max_y = -3.4028235e38f;
    }
    for (UnkGroupChild_ovb0066** it = self->begin(); it < self->end(); ++it) {
        if (!(*it)->enabled || !(*it)->vf_0x58())
            continue;
        UnkGroupChild_ovb0066* child = *it;
        if (child->dirty)
            child->vf_0x98();
        UnkBox_ovb0066& b = self->bounds;
        const UnkBox_ovb0066& c = child->bounds;
        f32 bx = b.min_x, by = b.min_y, cx = c.min_x, cy = c.min_y;
        b.min_x = bx < cx ? bx : cx;
        b.min_y = by < cy ? by : cy;
        bx = b.max_x, by = b.max_y, cx = c.max_x, cy = c.max_y;
        b.max_x = bx > cx ? bx : cx;
        b.max_y = by > cy ? by : cy;
    }
    if (self->bounds_reset) {
        self->last_bounds = self->bounds;
        self->bounds_reset = false;
    }
}
