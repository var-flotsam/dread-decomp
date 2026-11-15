// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eed7b0 applies a mask to the
// sub-area of the array at +0x860 that is named by an id (found by the pooled string's hash): it stores the
// first flag in the sub-area and hands it to fn_7100fac120. Nothing happens when none is found. Names and
// layouts are placeholders.
#include <algorithm>

#include "base/global/CStrId.h"
#include "../../game_s7/UnkIter.h"
#include "game/UnkMask.h"
#include "game/UnkTU2611.h"
#include "types.h"

using base::global::CStrId;

// The pooled string a CStrId points to, as far as its hash at +0x30.
struct UnkStrIdData_led7b0 {
    u8 pad_0x0[0x30];
    u64 hash;
};
static inline u64 HashOf(const CStrId& id) { return reinterpret_cast<const UnkStrIdData_led7b0*>(id.data)->hash; }

struct UnkSubAreaEntry_led7b0 {
    CStrId id;
    u8 pad_0x8[0x179];
    bool field_0x181;
    u8 pad_0x182[6];
};
static_assert(sizeof(UnkSubAreaEntry_led7b0) == 0x188, "layout");
static_assert(__builtin_offsetof(UnkSubAreaEntry_led7b0, field_0x181) == 0x181, "layout");

struct UnkSubAreaMgr_led7b0 {
    u8 pad_0x0[0x860];
    UnkSubAreaEntry_led7b0* entries;  // 0x860
    u32 count;                        // 0x868
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_led7b0, entries) == 0x860, "layout");

void fn_7100eed7b0(UnkSubAreaMgr_led7b0* self, const CStrId* id, const u32* mask, bool a, bool b, bool c) {
    UnkIter<UnkSubAreaEntry_led7b0> first{self->entries};
    UnkIter<UnkSubAreaEntry_led7b0> last{self->entries + self->count};
    UnkSubAreaEntry_led7b0* found =
        std::find_if(first, last, [id](const UnkSubAreaEntry_led7b0& e) { return HashOf(e.id) == HashOf(*id); }).p;
    if (self->entries + self->count == found)
        return;
    found->field_0x181 = a;
    fn_7100fac120(reinterpret_cast<UnkSubArea*>(found), UnkMask(*mask), a, b, nullptr, c);
}
