// Unit 354 (navmesh::CFixedDynamicSmartLinkRule's unit): moves the (index, value) pair an iterator
// value holds or refers to into the one another holds or refers to (no self-check).
// Names and layouts are placeholders.
#include "UnkNavRuleValues_A1_b75.h"
#include "base/UnkCollectionIter.h"
#include "types.h"

using UnkNavRuleIterValue_l272ddc = base::UnkIterValue<UnkNavRuleValue_A1_b75, u32>;

UnkNavRuleIterValue_l272ddc* fn_7100272ddc(UnkNavRuleIterValue_l272ddc* self, UnkNavRuleIterValue_l272ddc* other) {
    UnkNavRuleValue_A1_b75* src;
    if (other->state == 1) {
        self->Index() = other->value.index;
        src = &other->value.element;
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        src = other->ref.element;
    } else {
        return self;
    }
    self->Element() = static_cast<UnkNavRuleValue_A1_b75&&>(*src);
    return self;
}
