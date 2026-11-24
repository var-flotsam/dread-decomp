// CBlackboard's unit (tools/tu_map.py unit 2653). fn_71010482d4: removes the section in `slot`: from
// its group's sections first, then destroys it and removes it from the blackboard's own arrays,
// returning the slot that follows. Names and layouts are placeholders.
#include "base/UnkParallelFind.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;

using UnkSectionArray_b0026 = base::global::CRntSmallDictionary<CStrId, CBlackboard::CSection*>;

void fn_7101048420(CBlackboard::CSection* self);
CBlackboard::CSection** CBlackboard_SwapRemoveSectionArrayEntry(UnkSectionArray_b0026* self, CBlackboard::CSection** slot);

// The group a section belongs to (its field_0x10): the sections it holds at +0x38.
struct UnkSectionGroup_l0482d4 {
    u8 pad_0x0[0x38];
    UnkSectionArray_b0026 sections;  // 0x38
};

CBlackboard::CSection** fn_71010482d4(CBlackboard* self, CBlackboard::CSection** slot) {
    CBlackboard::CSection** next = &self->sections[self->count];
    if (slot != next) {
        CStrId key(self->ids[static_cast<u32>(slot - self->sections)]);
        if (auto* group = static_cast<UnkSectionGroup_l0482d4*>((*slot)->field_0x10)) {
            CBlackboard::CSection** found = UnkParallelFindSlot(&group->sections, key);
            if (found != group->sections.End())
                CBlackboard_SwapRemoveSectionArrayEntry(&group->sections, found);
        }
        if (CBlackboard::CSection* section = *slot) {
            fn_7101048420(section);
            operator delete(section);
            *slot = nullptr;
        }
        next = CBlackboard_SwapRemoveSectionArrayEntry(reinterpret_cast<UnkSectionArray_b0026*>(&self->sections), slot);
    }
    return next;
}
