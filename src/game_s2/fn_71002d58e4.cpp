// Forwards `dt` to virtual slot 0x70 of the object behind +0x38, then of each child in the array at +0xa0.
#include "types.h"

struct UnkTarget {
    void** vtable;
};

struct UnkParent;

struct UnkTargetRef {
    UnkTarget* target;
};

struct UnkParent {
    u8 pad_0x0[0x38];
    UnkTargetRef* field_0x38;
    u8 pad_0x40[0x60];
    UnkTarget** children;
    u32 count;
};

// NON_MATCHING(minor): the float load and the vtable load swap order in the loop.
void fn_71002d58e4(UnkParent* self, const float* dt) {
    UnkTarget* target = self->field_0x38->target;
    reinterpret_cast<void (*)(UnkTarget*, UnkParent*, u32, const float*)>(target->vtable[0x70 / 8])(target, self, 0, dt);
    UnkTarget** end = self->children + self->count;
    for (UnkTarget** p = self->children; p != end; ++p) {
        float v = *dt;
        reinterpret_cast<void (*)(UnkTarget*, float)>((*p)->vtable[0x70 / 8])(*p, v);
    }
}
