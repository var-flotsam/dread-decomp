// A property setter in the CCentralUnitComponent unit, as fn_71002d92b4: the byte picked from the
// argument's holder (inline, or behind a pointer for 9 or more entries or kind 1) is stored at 0x9f8 of
// the object at 8 when it differs. Returns an empty result of kind 2. Names and layouts are placeholders.
#include "types.h"


struct UnkValueSet_b35a2 {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_b35a2 {
    u8* holder;
    UnkValueSet_b35a2* set;
    u8 unk_0x10[4];
    s32 kind;
};
struct UnkCentralUnit {
    u8 unk_0x0[0x9f8];
    u8 field_0x9f8;
};
struct UnkCentralUnitOwner {
    u8 unk_0x0[8];
    UnkCentralUnit* target;
};
struct UnkResult_0x18_b35a2 {
    u64 empty[2];
    u64 kind_and_flag;
};

UnkResult_0x18_b35a2 fn_710077b878(void* unused, UnkCentralUnitOwner* owner, UnkValueSource_b35a2** source) {
    UnkValueSource_b35a2* src = *source;
    UnkCentralUnit* target = owner->target;
    u8* slot;
    if (src->kind == 1 || src->set->count >= 9)
        slot = src->holder;
    else
        slot = reinterpret_cast<u8*>(src);
    u8 value = *slot;
    if (target->field_0x9f8 != value)
        target->field_0x9f8 = value;
    UnkResult_0x18_b35a2 result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
