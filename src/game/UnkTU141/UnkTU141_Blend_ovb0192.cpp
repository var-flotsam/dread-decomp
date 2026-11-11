// Unit 141 of tools/tu_map.py (no reliable name): a blend node that applies one of its two sub-objects, or both
// blended by its weight, and the removal of an entry from a list of words. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "engine/render/anim/CSkeletalAnimTree.h"
#include "game/UnkTU23.h"
#include "types.h"

struct UnkBlendOut_k;

// A pair of sub-objects the blend writes into.
struct UnkBlendSub_k {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(void* target);
};

struct UnkBlendNode_k {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    virtual void vf_0x20();
    virtual UnkBlendSub_k* vf_0x28(bool second);

    u8 unk_0x8[0x30];
    f32 weight;  // 0x38
    u8 unk_0x3c[4];
    u8 first[0x38];   // 0x40
    u8 second[0x38];  // 0x78
};
static_assert(__builtin_offsetof(UnkBlendNode_k, weight) == 0x38, "layout");
static_assert(__builtin_offsetof(UnkBlendNode_k, second) == 0x78, "layout");

void fn_7100149da4(UnkBlendNode_k* self, void* target) {
    if (self->vf_0x18() != 2)
        return;
    f32 t = self->weight;
    if (t == 1.0f) {
        self->vf_0x28(true)->vf_0x38(target);
    } else if (t == 0.0f) {
        self->vf_0x28(false)->vf_0x38(target);
    } else {
        fn_7100014e5c(self->first, target);
        fn_7100014e5c(self->second, target);
        self->vf_0x28(false)->vf_0x38(self->first);
        self->vf_0x28(true)->vf_0x38(self->second);
        fn_710014b80c(target, self->first, self->second, nullptr, self->weight);
    }
}

struct UnkPtrListOwner_k {
    u8 unk_0x0[0x10];
    base::UnkVector<u64> items;  // 0x10
};

// Removes the first entry equal to `value`, shifting the later ones down; always true.
bool fn_710014a07c(UnkPtrListOwner_k* self, u64 value) {
    self->items.Remove(value);
    return true;
}
