// base::animation::CAnimationBlendSkeletonTransformer's unit: the destructor and the deleting
// destructor (the layout of UnkBlendTransformerCtor.cpp): the vtable, the shared pointer at 0xd0,
// the two members at 0x98 and 0x60 (destroyed out of line by fn_7100014e88), then the base part
// (fn_7100005ab0, CAnimationSkeletonTransformer_Dtor_b66a1.cpp); also the shared pointer's setter.
// Names and layouts are placeholders.
#include <memory>

#include "base/UnkVector.h"
#include "types.h"

void fn_7100014e88(void* member);
void fn_7100005ab0(void* self);

extern char g_unk_7101a19018[];

struct UnkBlendTransformerBase_b66a1 {
    char* vtable;
    u8 base[0x58];

    ~UnkBlendTransformerBase_b66a1() { fn_7100005ab0(this); }
};

struct UnkBlendTransformerMember_b66a1 {
    alignas(8) u8 data[0x38];

    ~UnkBlendTransformerMember_b66a1() { fn_7100014e88(this); }
};

struct UnkBlendTransformer_b66a1 : UnkBlendTransformerBase_b66a1 {
    UnkBlendTransformerMember_b66a1 member_0x60;
    UnkBlendTransformerMember_b66a1 member_0x98;
    std::shared_ptr<void> shared;  // 0xd0
    u64 field_0xe0;
    u64 field_0xe8;
    u64 field_0xf0;
    u64 field_0xf8;
    u8 field_0x100;

    ~UnkBlendTransformer_b66a1() { vtable = g_unk_7101a19018 + 0x10; }
};

void fn_7100003800(UnkBlendTransformer_b66a1* self) { self->~UnkBlendTransformer_b66a1(); }

// The deleting destructor: the same, then the game's operator delete.
void fn_7100003878(UnkBlendTransformer_b66a1* self) {
    self->~UnkBlendTransformer_b66a1();
    fn_7100083c00(self);
}

// Sets the shared pointer at 0xd0.
void fn_7100003a40(UnkBlendTransformer_b66a1* self, const std::shared_ptr<void>& shared) { self->shared = shared; }
