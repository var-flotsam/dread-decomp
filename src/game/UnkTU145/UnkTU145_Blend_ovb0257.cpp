// Unit 145 of tools/tu_map.py (no name hint). fn_710014ac40: the blend node of
// src/game/UnkTU141/UnkTU141_Blend_ovb0192.cpp (fn_7100149da4) applying its sub-objects, but with the
// first one applied before the weight decides: with a weight of 0 only that one; of 1 the second one
// too; otherwise the second one writes into a copy that is blended in by the weight. Names and
// layouts are placeholders.
#include "types.h"

void fn_7100014dfc(void* self, const void* other);
void fn_7100014e88(void* self);
void fn_710014b80c(void* target, void* from, void* to, void* extra, f32 t);

struct UnkBlendSub_b257d {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(void* target);
};

struct UnkBlendNode_b257d {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    virtual void vf_0x20();
    virtual UnkBlendSub_b257d* vf_0x28(bool second);

    u8 unk_0x8[0x30];
    f32 weight;  // 0x38
};

void fn_710014ac40(UnkBlendNode_b257d* self, void* target) {
    if (self->vf_0x18() != 2)
        return;
    f32 t = self->weight;
    if (t == 0.0f) {
        // keeps this call from being merged with the identical first call of the blended path (ordering only)
        asm("" : "+r"(target));
        self->vf_0x28(false)->vf_0x38(target);
        return;
    }
    self->vf_0x28(false)->vf_0x38(target);
    UnkBlendSub_b257d* second = self->vf_0x28(true);
    if (t == 1.0f) {
        second->vf_0x38(target);
        return;
    }
    u8 blended[0x38];
    fn_7100014dfc(blended, target);
    second->vf_0x38(blended);
    fn_710014b80c(target, target, blended, nullptr, self->weight);
    fn_7100014e88(blended);
}
