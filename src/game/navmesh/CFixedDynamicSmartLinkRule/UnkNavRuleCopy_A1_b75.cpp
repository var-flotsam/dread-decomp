// Unit 354: the copy constructor and copy assignment of the array in UnkNavRuleValues_A1_b75.h.
#include "UnkNavRuleValues_A1_b75.h"

void fn_71002726e8(UnkNavRuleValues_A1_b75* self, const UnkNavRuleValues_A1_b75* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100272474(reinterpret_cast<UnkSmartLinkEntryArray*>(self), other->capacity);
    const UnkNavRuleValue_A1_b75* it = other->data;
    const UnkNavRuleValue_A1_b75* end = it + other->count;
    for (; it != end; ++it)
        self->PushBackCopy(*it);
}

UnkNavRuleValues_A1_b75* fn_7100272348(UnkNavRuleValues_A1_b75* self, const UnkNavRuleValues_A1_b75* other) {
    if (self != other) {
        self->Clear();
        fn_7100272474(reinterpret_cast<UnkSmartLinkEntryArray*>(self), other->capacity);
        const UnkNavRuleValue_A1_b75* it = other->data;
        const UnkNavRuleValue_A1_b75* end = it + other->count;
        for (; it != end; ++it)
            self->PushBackCopy(*it);
    }
    return self;
}
