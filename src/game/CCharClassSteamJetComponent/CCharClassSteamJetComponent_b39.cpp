// CCharClassSteamJetComponent unit (tools/tu_map.py, the name from reflection): a bool property setter:
// stores the flag at +0xc08 and +0xae1 and its negation at +0xae3 and +0xad8. Names and layouts are placeholders.
#include "types.h"

struct UnkValueSet_sj {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_sj {
    void* holder;
    UnkValueSet_sj* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_sj {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkTarget_sj {
    u8 unk_0x0[0xad8];
    bool field_0xad8;
    u8 unk_0xad9[8];
    bool field_0xae1;
    u8 unk_0xae2;
    bool field_0xae3;
    u8 unk_0xae4[0x124];
    bool field_0xc08;
};

struct UnkOwner_sj {
    u8 unk_0x0[8];
    UnkTarget_sj* target;
};

UnkResult_0x18_sj fn_710089fff0(void*, UnkOwner_sj* owner, UnkValueSource_sj** source) {
    UnkValueSource_sj* src = *source;
    UnkTarget_sj* target = owner->target;
    bool value;
    if (src->kind == 1 || src->set->count >= 9)
        value = *static_cast<bool*>(src->holder);
    else
        value = *reinterpret_cast<bool*>(src);
    bool inv = !value;
    target->field_0xc08 = value;
    target->field_0xae1 = value;
    target->field_0xae3 = inv;
    target->field_0xad8 = inv;
    UnkResult_0x18_sj result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
