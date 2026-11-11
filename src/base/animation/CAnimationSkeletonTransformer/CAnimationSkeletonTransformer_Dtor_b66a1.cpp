// base::animation::CAnimationSkeletonTransformer's unit: the destructor and the deleting destructor
// (the layout of CAnimationSkeletonTransformer_Ctor_b49.cpp): the vtable, then the member at 0x28
// (destroyed out of line by fn_7100014e88) and the shared pointer, in reverse order of declaration.
// Names and layouts are placeholders.
#include <memory>

#include "base/UnkVector.h"
#include "game/UnkTU23.h"
#include "types.h"

extern char g_unk_7101a190e8[];

struct UnkSkeletonTransformerMember_b66a1 {
    alignas(8) u8 data[0x38];

    ~UnkSkeletonTransformerMember_b66a1() { fn_7100014e88(this); }
};

struct UnkSkeletonTransformer_b66a1 {
    char* vtable;
    std::shared_ptr<void> shared;
    u64 field_0x18;
    u16 field_0x20;
    u8 field_0x22;
    UnkSkeletonTransformerMember_b66a1 member_0x28;

    ~UnkSkeletonTransformer_b66a1() { vtable = g_unk_7101a190e8 + 0x10; }
};
static_assert(sizeof(UnkSkeletonTransformer_b66a1) == 0x60, "UnkSkeletonTransformer_b66a1 layout");

// Signature as CAnimationBlendSkeletonTransformer_Dtor_b66a1.cpp declares it.
void fn_7100005ab0(void* self) { static_cast<UnkSkeletonTransformer_b66a1*>(self)->~UnkSkeletonTransformer_b66a1(); }

// The deleting destructor: the same, then the game's operator delete.
void fn_7100005b20(void* self) {
    static_cast<UnkSkeletonTransformer_b66a1*>(self)->~UnkSkeletonTransformer_b66a1();
    fn_7100083c00(self);
}
