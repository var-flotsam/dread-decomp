// In the engine::render::anim::CSkeletalAnimTree unit: stores a pointer at +0x70 and passes it on to
// every child (an array of pointers at +0x80, count at +0x88) through the child's slot 0x50.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSkeletalAnimTreeChild {
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
    virtual void vf_0x50(void* owner);
};

struct UnkSkeletalAnimTreeNode {
    u8 pad_0x0[0x70];
    void* owner;                          // 0x70
    u8 pad_0x78[0x8];
    UnkSkeletalAnimTreeChild** children;  // 0x80
    u32 count;                            // 0x88
};

void fn_710014bb04(UnkSkeletalAnimTreeNode* self, void* owner) {
    self->owner = owner;
    for (UnkSkeletalAnimTreeChild **it = self->children, **end = it + self->count; it != end; ++it)
        (*it)->vf_0x50(owner);
}
