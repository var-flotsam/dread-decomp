// base::animation::CAnimationBlendSkeletonTransformer's reflected copy constructors (slots 0x30 and
// 0x38 of src/reflection/base_animation_CAnimationBlendSkeletonTransformer.cpp, the same code): the
// CAnimationSkeletonTransformer part (as in src/base/animation/CAnimationSkeletonTransformer/
// CAnimationSkeletonTransformer_Ctor_b49.cpp), the vtable, two more members copied by fn_7100014dfc,
// a shared pointer and plain fields (UnkBlendTransformerCtor.cpp's constructor). Names and layouts
// are placeholders.
#include <memory>
#include <new>

#include "base/animation/CAnimationBlendSkeletonTransformer.h"
#include "game/UnkTU23.h"
#include "types.h"

extern char g_unk_7101a190e8[];

struct UnkBlendMember_b59 {
    alignas(8) u8 data[0x38];

    UnkBlendMember_b59(const UnkBlendMember_b59& other) { fn_7100014dfc(this, &other); }
};

struct UnkBlendSkeletonBase_b59 {
    char* vtable;

    UnkBlendSkeletonBase_b59(const UnkBlendSkeletonBase_b59&) : vtable(g_unk_7101a190e8 + 0x10) {}
};

struct UnkBlendSkeletonData_b59 : UnkBlendSkeletonBase_b59 {
    std::shared_ptr<void> shared;
    u64 field_0x18;
    u16 field_0x20;
    u8 field_0x22;
    UnkBlendMember_b59 member_0x28;
};

// Two words copied as a pair.
struct UnkBlendPair_b59 {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkBlendTransformer_b59 : UnkBlendSkeletonData_b59 {
    UnkBlendMember_b59 member_0x60;
    UnkBlendMember_b59 member_0x98;
    std::shared_ptr<void> shared_0xd0;
    UnkBlendPair_b59 field_0xe0;
    u64 field_0xf0;
    u64 field_0xf8;
    u8 field_0x100;

    UnkBlendTransformer_b59(const UnkBlendTransformer_b59& other)
        : UnkBlendSkeletonData_b59(other), member_0x60((vtable = g_unk_7101a19018 + 0x10, other.member_0x60)), member_0x98(other.member_0x98),
          shared_0xd0(other.shared_0xd0), field_0xe0(other.field_0xe0),
          field_0xf0(other.field_0xf0), field_0xf8(other.field_0xf8), field_0x100(other.field_0x100) {}
};
static_assert(sizeof(UnkBlendTransformer_b59) == 0x108, "UnkBlendTransformer_b59 layout");

void fn_7100003eb0(void* dst, const void* src) {
    new (dst) UnkBlendTransformer_b59(*static_cast<const UnkBlendTransformer_b59*>(src));
}
void fn_7100003f94(void* dst, const void* src) {
    new (dst) UnkBlendTransformer_b59(*static_cast<const UnkBlendTransformer_b59*>(src));
}
