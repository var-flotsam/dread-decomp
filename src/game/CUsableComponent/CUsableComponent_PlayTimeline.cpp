// A property setter in the CUsableComponent unit (unit 1607 of tools/tu_map.py): the bool picked from
// the argument's holder (inline, or behind a pointer for 9 or more entries or kind 1) chooses one of two
// timeline ids (constants in .rodata) for the actor at +0x30 of the object at 8 (fn_7100d7c828). Returns
// an empty result of kind 2. Names and layouts are placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern const u64 g_unk_710164b648[];
extern const u64 g_unk_710164b658[];

struct UnkValueSet_b38a3u {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_b38a3u {
    bool* holder;
    UnkValueSet_b38a3u* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkUsable_b38a3 {
    u8 unk_0x0[0x30];
    UnkActor* actor;
};

struct UnkUsableOwner_b38a3 {
    u8 unk_0x0[8];
    UnkUsable_b38a3* target;
};

struct UnkResult_0x18_b38a3u {
    u64 empty[2];
    u64 kind_and_flag;
};

UnkResult_0x18_b38a3u fn_7100aa42a8(void* unused, UnkUsableOwner_b38a3* owner,
                                    UnkValueSource_b38a3u** source) {
    UnkUsable_b38a3* target = owner->target;
    UnkValueSource_b38a3u* src = *source;
    bool* slot;
    if (src->kind == 1 || src->set->count >= 9)
        slot = src->holder;
    else
        slot = reinterpret_cast<bool*>(src);
    bool on = *slot;
    fn_7100d7c828(target->actor, on ? g_unk_710164b658 : g_unk_710164b648, false);
    UnkResult_0x18_b38a3u result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
