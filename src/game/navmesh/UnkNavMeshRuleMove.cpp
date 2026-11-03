// From unit 354 of tools/tu_map.py (navmesh::CFixedDynamicSmartLinkRule's unit): the move
// assignment of the reflected 0x50-byte value described by fn_710026d954 (its descriptor's
// fn_0x50). Four words at +0x0, an array of 0x50-byte entries at +0x10 (destroyed by
// fn_710026ead0, taken over from the source) and a 0x15-byte block at +0x38. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_710026ead0(void* self);

namespace {
struct UnkNavRuleEntry {
    u8 bytes[0x50];
    ~UnkNavRuleEntry() { fn_710026ead0(this); }
};

// A word with its own assignment, so the four at +0x0 are copied one by one.
struct UnkNavRuleWord {
    u32 value;
    UnkNavRuleWord& operator=(const UnkNavRuleWord& other) {
        value = other.value;
        return *this;
    }
};

struct UnkNavRuleValue {
    UnkNavRuleWord field_0x0;
    UnkNavRuleWord field_0x4;
    UnkNavRuleWord field_0x8;
    UnkNavRuleWord field_0xc;
    base::UnkVector<UnkNavRuleEntry> entries;  // 0x10
    u8 field_0x38[0x15];

    UnkNavRuleValue& operator=(UnkNavRuleValue&& other) = default;
};
static_assert(sizeof(UnkNavRuleValue) == 0x50, "UnkNavRuleValue layout");
}  // namespace

void* fn_7100272a6c(void* dst, const void* src) {
    UnkNavRuleValue* d = static_cast<UnkNavRuleValue*>(dst);
    UnkNavRuleValue* s = static_cast<UnkNavRuleValue*>(const_cast<void*>(src));
    if (d != s)
        *d = static_cast<UnkNavRuleValue&&>(*s);
    return d;
}
