// Unit 354: slots 0x140 and 0x148 (set element at index) of the array in UnkNavRuleValues_A1_b75.h.
#include "UnkNavRuleValues_A1_b75.h"

void fn_7100273240(UnkNavRuleValues_A1_b75* self, const u32* index, const UnkNavRuleValue_A1_b75* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        navmesh::Grow_A1_b75(self, i);
    UnkNavRuleValue_A1_b75* d = &self->data[i];
    asm("" : "+r"(d));
    *d = *value;
}

void fn_71002733b0(UnkNavRuleValues_A1_b75* self, const u32* index, UnkNavRuleValue_A1_b75* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        navmesh::Grow_A1_b75(self, i);
    self->data[i] = static_cast<UnkNavRuleValue_A1_b75&&>(*value);
}
