// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eed3a4 updates a mask of the
// sub-area named by an id (found in the array at +0x860 by the pooled string's hash), while no change is running
// (+0x110 clear): fn_7100facf04 sets or clears it; when it was set and the sub-area is the current one (+0x3f0)
// the mask is applied to it (fn_7100fac120), otherwise it is applied with the current sub-area as the other
// one. Names and layouts are placeholders.
#include <algorithm>

#include "base/global/CStrId.h"
#include "../../../src/game_s7/UnkIter.h"
#include "game/UnkMask.h"
#include "game/UnkTU2611.h"
#include "game/UnkTU2577.h"
#include "types.h"

using base::global::CStrId;

// A bit mask passed by value (copied into a temporary), as in
// src/game/CAnimatedCameraCtrl/CAnimatedCameraCtrl_Lifecycle_ovb0101.cpp.
struct UnkCamMask_b0101 {
    u32 bits;
    UnkCamMask_b0101() : bits(0) {}
    UnkCamMask_b0101(u32 b) : bits(b) {}
    UnkCamMask_b0101(const UnkCamMask_b0101& o) : bits(o.bits) {}
};

void fn_7100facf04(UnkSubArea* area, UnkMask mask, bool on);

// The pooled string a CStrId points to, as far as its hash at +0x30.
struct UnkStrIdData_led3a4 {
    u8 pad_0x0[0x30];
    u64 hash;
};
static inline u64 HashOf(const CStrId& id) { return reinterpret_cast<const UnkStrIdData_led3a4*>(id.data)->hash; }

struct UnkSubAreaEntry_led3a4 {
    CStrId id;
    u8 pad_0x8[0x180];
};
static_assert(sizeof(UnkSubAreaEntry_led3a4) == 0x188, "layout");

struct UnkSubAreaMgr_led3a4 {
    u8 pad_0x0[0x110];
    u32 field_0x110;
    u8 pad_0x114[0x2dc];
    UnkSubAreaEntry_led3a4* current;  // 0x3f0
    u8 pad_0x3f8[0x468];
    UnkSubAreaEntry_led3a4* entries;  // 0x860
    u32 count;                        // 0x868

};
static_assert(__builtin_offsetof(UnkSubAreaMgr_led3a4, current) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_led3a4, entries) == 0x860, "layout");

void fn_7100eed3a4(void* manager, const CStrId* id, UnkCamMask_b0101 mask, bool flag) {
    UnkSubAreaMgr_led3a4* self = static_cast<UnkSubAreaMgr_led3a4*>(manager);
    if (self->field_0x110 != 0)
        return;
    UnkIter<UnkSubAreaEntry_led3a4> first{self->entries};
    UnkIter<UnkSubAreaEntry_led3a4> last{self->entries + self->count};
    UnkSubAreaEntry_led3a4* found =
        std::find_if(first, last, [id](const UnkSubAreaEntry_led3a4& e) { return HashOf(e.id) == HashOf(*id); }).p;
    if (found == self->entries + self->count)
        return;
    UnkSubArea* area = reinterpret_cast<UnkSubArea*>(found);
    fn_7100facf04(area, UnkMask(mask.bits), flag);
    if (flag)
        return;
    if (self->current == found)
        fn_7100fac120(area, UnkMask(mask.bits), true, false, nullptr, false);
    else
        fn_7100fac120(area, UnkMask(mask.bits), false, false, reinterpret_cast<UnkSubArea*>(self->current), false);
}
