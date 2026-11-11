// game::logic::collision::CCollider's unit: changing a collider's mask at +0x64. A group (inline list
// of children at +0x164: a u32 count followed directly by the pointers) forwards the change to every
// child (slot 0xa0); a leaf tells its owner at +0x30 the old and the new mask. Names and layouts are
// placeholders.
#include "game/UnkTU2510.h"
#include "game/logic/collision/CBroadphaseObject.h"
#include "types.h"

struct UnkCollider_A6_b70 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(u32 what);
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
};

#pragma pack(push, 4)
struct UnkColliderNode_A6_b70 : UnkCollider_A6_b70 {
    u8 pad_0x8[0x30 - 0x8];
    void* owner;
    u8 pad_0x38[0x64 - 0x38];
    u32 mask;
    u8 pad_0x68[0x164 - 0x68];
    u32 count;
    UnkCollider_A6_b70* children[1];  // 0x168, 4-byte aligned
};
#pragma pack(pop)

void fn_7100d35b00(UnkColliderNode_A6_b70* self, u32 mask);

void fn_7100d370e4(UnkColliderNode_A6_b70* self, u32 mask) {
    u32 old_mask = self->mask;
    if (old_mask == mask)
        return;
    if (old_mask == 0) {
        fn_7100d35af4(self);
        old_mask = self->mask;
    }
    fn_7100d35b00(self, mask);
    self->vf_0x40(2);
    if (self->count != 0) {
        u64 bytes = (u64)self->count * sizeof(void*);
        asm("" : "+r"(bytes));
        if (bytes == 0)
            return;
        UnkCollider_A6_b70** begin = self->children;
        asm("" : "+r"(begin));
        UnkCollider_A6_b70** it = begin;
        do {
            (*it)->vf_0xa0();
            ++it;
        } while (it != begin + self->count);
    } else if (self->owner) {
        fn_7100d5c888(self->owner, self, old_mask, mask);
    }
}
