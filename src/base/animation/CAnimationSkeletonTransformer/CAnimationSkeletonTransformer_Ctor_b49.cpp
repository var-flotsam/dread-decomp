// base::animation::CAnimationSkeletonTransformer's unit: its constructor (fn_7100005a88, the base
// part of CAnimationBlendSkeletonTransformer's, src/base/animation/
// CAnimationBlendSkeletonTransformer/UnkBlendTransformerCtor.cpp) and the reflected class's
// helpers (src/reflection/base_animation_CAnimationSkeletonTransformer.cpp): construct in place
// (slot 0x28, byte-identical to the constructor), copy-construct (slots 0x30 and 0x38, the same
// code) and copy-assign (slots 0x48 and 0x50, the same code). The object holds a shared pointer,
// plain fields copied as one block, and a member at 0x28 built, copied and assigned out of line.
// Names and layouts are placeholders.
#include <memory>
#include <new>

#include "types.h"

void fn_7100014d94(void* self, u32 arg);
void fn_7100014dfc(void* self, const void* other);
void fn_7100014e5c(void* self, const void* other);

extern char g_unk_7101a190e8[];

struct UnkSkeletonTransformerMember_b49a1 {
    alignas(8) u8 data[0x38];

    explicit UnkSkeletonTransformerMember_b49a1(u32 arg) { fn_7100014d94(this, arg); }
    UnkSkeletonTransformerMember_b49a1(const UnkSkeletonTransformerMember_b49a1& other) {
        fn_7100014dfc(this, &other);
    }
    UnkSkeletonTransformerMember_b49a1& operator=(const UnkSkeletonTransformerMember_b49a1& other) {
        fn_7100014e5c(this, &other);
        return *this;
    }
};

// The vtable, stored by every constructor (copies included) and left alone by assignment.
struct UnkSkeletonTransformerBase_b49a1 {
    char* vtable;

    UnkSkeletonTransformerBase_b49a1() : vtable(g_unk_7101a190e8 + 0x10) {}
    UnkSkeletonTransformerBase_b49a1(const UnkSkeletonTransformerBase_b49a1&) : vtable(g_unk_7101a190e8 + 0x10) {}
    UnkSkeletonTransformerBase_b49a1& operator=(const UnkSkeletonTransformerBase_b49a1&) { return *this; }
};

// The members, with the implicit copy and assignment: the plain fields are copied as one block
// (the implicit copy's memcpy, up to the last field).
struct UnkSkeletonTransformerData_b49a1 : UnkSkeletonTransformerBase_b49a1 {
    std::shared_ptr<void> shared;
    u64 field_0x18;
    u16 field_0x20;
    u8 field_0x22;
    UnkSkeletonTransformerMember_b49a1 member_0x28;

    UnkSkeletonTransformerData_b49a1() : field_0x18(0), field_0x20(0), field_0x22(1), member_0x28(0) {}
};

// Assignment skips self-assignment, then assigns the members.
struct UnkSkeletonTransformer_b49a1 : UnkSkeletonTransformerData_b49a1 {
    UnkSkeletonTransformer_b49a1() {}
    UnkSkeletonTransformer_b49a1(const UnkSkeletonTransformer_b49a1& other) = default;

    UnkSkeletonTransformer_b49a1& operator=(const UnkSkeletonTransformer_b49a1& other) {
        if (this != &other)
            UnkSkeletonTransformerData_b49a1::operator=(other);
        return *this;
    }
};
static_assert(sizeof(UnkSkeletonTransformer_b49a1) == 0x60, "UnkSkeletonTransformer_b49a1 layout");

// Signature as UnkBlendTransformerCtor.cpp declares it.
void fn_7100005a88(void* self) { new (self) UnkSkeletonTransformer_b49a1(); }

void fn_7100005f8c(void* self) { new (self) UnkSkeletonTransformer_b49a1(); }

void fn_7100005fb4(void* dst, const void* src) {
    new (dst) UnkSkeletonTransformer_b49a1(*static_cast<const UnkSkeletonTransformer_b49a1*>(src));
}

void fn_710000600c(void* dst, const void* src) {
    new (dst) UnkSkeletonTransformer_b49a1(*static_cast<const UnkSkeletonTransformer_b49a1*>(src));
}

void* fn_7100006070(void* dst, const void* src) {
    return &(*static_cast<UnkSkeletonTransformer_b49a1*>(dst) = *static_cast<const UnkSkeletonTransformer_b49a1*>(src));
}

void* fn_7100006118(void* dst, const void* src) {
    return &(*static_cast<UnkSkeletonTransformer_b49a1*>(dst) = *static_cast<const UnkSkeletonTransformer_b49a1*>(src));
}
