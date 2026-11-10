// base::animation::CAnimationSkeletonTransformer's unit: sets the transformer's shared animation
// (+0x8, the layout of CAnimationSkeletonTransformer_Ctor_b49.cpp): a new one is reset to time 0
// (its vf_0x48) and marks +0x22; +0x21 is cleared. The flag and index the callers pass are unused.
// Names and layouts are placeholders.
#include <memory>

#include "types.h"

struct UnkSkeletonAnim_ovb0069 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(f32 time);
};

struct UnkSkeletonTransformerSet_ovb0069 {
    char* vtable;
    std::shared_ptr<void> shared;  // 0x8
    u64 field_0x18;
    u8 field_0x20;
    bool field_0x21;
    bool field_0x22;
};

void fn_7100005b9c(void* obj, std::shared_ptr<void> value, bool flag, s64 index) {
    auto* self = static_cast<UnkSkeletonTransformerSet_ovb0069*>(obj);
    self->shared = value;
    if (self->shared) {
        static_cast<UnkSkeletonAnim_ovb0069*>(self->shared.get())->vf_0x48(0.0f);
        self->field_0x22 = true;
    }
    self->field_0x21 = false;
}
