// CBlackboard's unit: copies another blackboard's sections into this one (after clearing it), each
// section a new copy added under the other's key. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;

void fn_7100af0030(UnkSectionData* self, const UnkSectionData* other);

bool fn_7101047068(CBlackboard* self, CBlackboard* other) {
    self->vf_0x18();
    if (!other)
        return false;
    for (CBlackboard::CSection **it = other->sections, **end = it + other->count; it != end;) {
        CBlackboard::CSection** slot = it++;
        CBlackboard::CSection* src = *slot;
        if (!src)
            continue;
        auto* section = static_cast<CBlackboard::CSection*>(fn_71000839a0(sizeof(CBlackboard::CSection), 0x15));
        new (&section->key) CStrId(src->key);
        section->blackboard = self;
        section->field_0x10 = nullptr;
        fn_7100af0030(&section->data, &src->data);
        section->data.field_0x38 = src->data.field_0x38;
        u32 i = slot - other->sections;
        self->vf_0x28(other->ids[i], section);
    }
    return false;
}