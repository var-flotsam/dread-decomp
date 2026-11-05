// CTriggerComponent unit (tools/tu_map.py, unit 1578): whether a packed list of 0x18-byte records (u32 count at
// +0x60, records from +0x64) holds one whose object's target (+0x10, then +0x30) has the given string's key (+0x30).
// Each visited target's counter at +0x44 is bumped first. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

// The bump goes through a may_alias type: the original reloads the target after it. The key is read through a
// volatile access: the original loads it from the stack copy on every iteration.
typedef s32 __attribute__((may_alias)) UnkAliasS32_A5_b72;

namespace {
struct UnkTriggerTarget_A5_b72 {
    u8 pad_0x0[0x30];
    s64 key;
    u8 pad_0x38[0xc];
    s32 visits;
};

struct UnkTriggerInner_A5_b72 {
    u8 pad_0x0[0x30];
    UnkTriggerTarget_A5_b72* target;
};

struct UnkTriggerObj_A5_b72 {
    u8 pad_0x0[0x10];
    UnkTriggerInner_A5_b72* inner;
};
}  // namespace

#pragma pack(push, 4)
struct UnkTriggerRecord_A5_b72 {
    UnkTriggerObj_A5_b72* obj;
    u8 pad_0x8[0x10];
};

struct UnkTriggerList_A5_b72 {
    u8 pad_0x0[0x60];
    u32 count;
    UnkTriggerRecord_A5_b72 records[1];
};
#pragma pack(pop)
static_assert(sizeof(UnkTriggerRecord_A5_b72) == 0x18, "record");

static inline __attribute__((always_inline)) UnkTriggerRecord_A5_b72* Find_A5_b72(
    UnkTriggerRecord_A5_b72* it, UnkTriggerRecord_A5_b72* end, const base::global::CRntString& key) {
    for (; it != end; ++it) {
        if (UnkTriggerObj_A5_b72* obj = it->obj) {
            if (UnkTriggerInner_A5_b72* inner = obj->inner) {
                ++*reinterpret_cast<UnkAliasS32_A5_b72*>(&inner->target->visits);
                if (inner->target->key == *(volatile s64*)&key.key)
                    break;
            }
        }
    }
    return it;
}

bool fn_7100a60104(UnkTriggerList_A5_b72* self, const base::global::CRntString& name) {
    UnkTriggerRecord_A5_b72* begin = self->records;
    asm("" : "+r"(begin));
    u64 bytes = (u64)self->count * sizeof(UnkTriggerRecord_A5_b72);
    asm("" : "+r"(bytes));
    base::global::CRntString key(name);
    UnkTriggerRecord_A5_b72* end = reinterpret_cast<UnkTriggerRecord_A5_b72*>(reinterpret_cast<char*>(begin) + bytes);
    return Find_A5_b72(begin, end, key) != begin + self->count;
}
