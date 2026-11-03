// behaviortree::CSetCrazyEndAnimAction's unit (tools/tu_map.py unit 453): the string id at +0x168
// of the record that the owner's (+0x18) +0x240 object holds at +0x1610, or the global empty id when
// a link is missing. Names are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRecord453 {
    u8 pad_0x0[0x168];
    base::global::CStrId id;  // 0x168
};

struct UnkInner453 {
    u8 pad_0x0[0x1610];
    UnkRecord453* record;  // 0x1610
};

struct UnkOwner453 {
    u8 pad_0x0[0x240];
    UnkInner453* inner;  // 0x240
};

struct UnkAction453 {
    u8 pad_0x0[0x18];
    UnkOwner453* owner;  // 0x18
};

const base::global::CStrId* fn_7100370fa0(const UnkAction453* self) {
    UnkInner453* inner = self->owner->inner;
    if (inner == nullptr) {
        const base::global::CStrId* none = &g_unk_7101d470e0;
        asm("" : "+r"(none));
        return none;
    }
    UnkRecord453* record = inner->record;
    return record != nullptr ? &record->id : &g_unk_7101d470e0;
}
