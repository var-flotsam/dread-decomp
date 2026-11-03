// A property setter in the CDoorShieldLifeComponent unit, as fn_71002d92b4: the float picked from the
// argument's holder (inline, or behind a pointer for 9 or more entries or kind 1) is stored at 0x410
// of the object at 8 when it is not negative. Returns an empty result of kind 2. Names and layouts are
// placeholders.
#include "types.h"

struct UnkFloatSet_b35a2 {
    u8 unk_0x0[0x18];
    u64 count;
};
struct UnkFloatSource_b35a2 {
    f32* holder;
    UnkFloatSet_b35a2* set;
    u8 unk_0x10[4];
    s32 kind;
};
struct UnkDoorShieldLife {
    u8 unk_0x0[0x410];
    f32 field_0x410;
};
struct UnkDoorShieldLifeOwner {
    u8 unk_0x0[8];
    UnkDoorShieldLife* target;
};
struct UnkResult_0x18_b35a2d {
    u64 empty[2];
    u64 kind_and_flag;
};

UnkResult_0x18_b35a2d fn_71008dc05c(void* unused, UnkDoorShieldLifeOwner* owner, UnkFloatSource_b35a2** source) {
    UnkFloatSource_b35a2* src = *source;
    UnkDoorShieldLife* target = owner->target;
    f32* slot;
    if (src->kind == 1 || src->set->count >= 9)
        slot = src->holder;
    else
        slot = reinterpret_cast<f32*>(src);
    f32 value = *slot;
    if (value >= 0.0f)
        target->field_0x410 = value;
    UnkResult_0x18_b35a2d result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
