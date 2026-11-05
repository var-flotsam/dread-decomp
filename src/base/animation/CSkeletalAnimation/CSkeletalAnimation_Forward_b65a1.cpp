// base::animation::CSkeletalAnimation unit (tools/tu_map.py, the name from reflection): passes two
// arguments on to the virtual at slot 0x30 of the object held at +0x8. Names are placeholders.
#include "types.h"

struct UnkSkeletalTarget_b65a1 {
    void** vtable;
};

struct UnkSkeletalOwner_b65a1 {
    u8 pad_0x0[0x8];
    UnkSkeletalTarget_b65a1* target;
};

void fn_7100013c74(UnkSkeletalOwner_b65a1* self, u64 a, u64 b) {
    typedef void (*Fn)(UnkSkeletalTarget_b65a1*, u64, u64);
    UnkSkeletalTarget_b65a1* target = self->target;
    reinterpret_cast<Fn>(target->vtable[0x30 / 8])(target, a, b);
}
