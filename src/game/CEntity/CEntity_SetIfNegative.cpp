// A property setter in the CEntity unit (unit 2523 of tools/tu_map.py), shaped like fn_710077b878:
// the float picked from the argument's holder (inline, or behind a pointer for 9 or more entries or
// kind 1) is stored at 0x3ac of the object at 8 only while that field is negative. Returns an empty
// result of kind 2. Names and layouts are placeholders.
#include "types.h"


struct UnkValueSet_b38a3 {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_b38a3 {
    f32* holder;
    UnkValueSet_b38a3* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkEntityTimed_b38a3 {
    u8 unk_0x0[0x3ac];
    f32 field_0x3ac;
};

struct UnkEntityTimedOwner_b38a3 {
    u8 unk_0x0[8];
    UnkEntityTimed_b38a3* target;
};

struct UnkResult_0x18_b38a3 {
    u64 empty[2];
    u64 kind_and_flag;
};

UnkResult_0x18_b38a3 fn_7100d82678(void* unused, UnkEntityTimedOwner_b38a3* owner,
                                   UnkValueSource_b38a3** source) {
    UnkEntityTimed_b38a3* target = owner->target;
    UnkValueSource_b38a3* src = *source;
    f32* slot;
    if (src->kind == 1 || src->set->count >= 9)
        slot = src->holder;
    else
        slot = reinterpret_cast<f32*>(src);
    f32 value = *slot;
    if (target->field_0x3ac < 0.0f)
        target->field_0x3ac = value;
    UnkResult_0x18_b38a3 result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
