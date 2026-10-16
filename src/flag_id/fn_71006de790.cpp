// Constructor with no base: vtable, two scalar fields, and a member built with arguments.
#include "types.h"

struct UnkMember_710059c644 {
    UnkMember_710059c644(s32 a, s32 b, s32 c, void* d);
    void* unk_0x0;
};

struct Unk_fn_71006de790 {
    Unk_fn_71006de790();
    virtual ~Unk_fn_71006de790();
    u64 field_0x8;
    f32 field_0x10;
    UnkMember_710059c644 field_0x18;
};

Unk_fn_71006de790::Unk_fn_71006de790()
    : field_0x8(0), field_0x10(-1.0f), field_0x18(8, 0, 0x13, nullptr) {}
