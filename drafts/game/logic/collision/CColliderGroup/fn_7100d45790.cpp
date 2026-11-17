// game::logic::collision::CColliderGroup (unit 2504 of tools/tu_map.py): returns the group's transform at
// +0x6c, first recomputing it when it is out of date: the previous transform keeps the old one, and the
// translation becomes the mean of the children's. Names and layouts are placeholders.
#include "types.h"

// A 3x4 transform (rows, translation in the last column) and four more words, the middle two one 8-byte value.
#pragma pack(push, 4)
struct UnkColliderGroupTransform_ld45790 {
    f32 m[3][4];
    f32 field_0x30;
    u64 field_0x34;
    f32 field_0x3c;

    UnkColliderGroupTransform_ld45790& operator=(const UnkColliderGroupTransform_ld45790& other) {
        for (int row = 0; row < 3; ++row) {
            m[row][0] = other.m[row][0];
            m[row][1] = other.m[row][1];
            m[row][2] = other.m[row][2];
            m[row][3] = other.m[row][3];
        }
        field_0x30 = other.field_0x30;
        field_0x34 = other.field_0x34;
        field_0x3c = other.field_0x3c;
        return *this;
    }
};
#pragma pack(pop)

struct UnkColliderGroupChild_ld45790 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual const UnkColliderGroupTransform_ld45790* vf_0x70();
};

#pragma pack(push, 4)

struct UnkColliderGroup_ld45790 {
    u8 pad_0x0[0x68];
    bool previous_stale;                          // 0x68
    UnkColliderGroupTransform_ld45790 current;    // 0x6c
    UnkColliderGroupTransform_ld45790 previous;   // 0xac
    u8 pad_0xec[0x124 - 0xec];
    u32 child_count;                              // 0x124
    UnkColliderGroupChild_ld45790* children[64];  // 0x128
    bool dirty;                                   // 0x328
};
#pragma pack(pop)
static_assert(__builtin_offsetof(UnkColliderGroup_ld45790, children) == 0x128, "layout");
static_assert(__builtin_offsetof(UnkColliderGroup_ld45790, dirty) == 0x328, "layout");

UnkColliderGroupTransform_ld45790* fn_7100d45790(UnkColliderGroup_ld45790* self) {
    if (self->dirty) {
        self->previous = self->current;
        self->dirty = false;
        f32 x = 0.0f;
        f32 y = 0.0f;
        f32 z = 0.0f;
        // The inline list at +0x124: a u32 count followed directly by the pointers; the count is re-read
        // after every call.
        u64 begin = reinterpret_cast<u64>(self->children);
        for (u64 it = begin; it < begin + (u64)self->child_count * 8; it += 8) {
            const UnkColliderGroupTransform_ld45790* child =
                (*reinterpret_cast<UnkColliderGroupChild_ld45790**>(it))->vf_0x70();
            x += child->m[0][3];
            y += child->m[1][3];
            z += child->m[2][3];
        }
        f32 count = self->child_count;
        self->current.m[0][3] = x / count;
        self->current.m[1][3] = y / count;
        self->current.m[2][3] = z / count;
        if (self->previous_stale) {
            self->previous = self->current;
            self->previous_stale = false;
        }
    }
    return &self->current;
}
