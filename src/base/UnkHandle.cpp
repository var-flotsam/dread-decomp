// The handle's out-of-line destructor and target forwarding.
#include "base/UnkHandle.h"

using base::UnkHandle;

void fn_7100084260(UnkHandle* self) {
    self->~UnkHandle();
}

base::UnkHandleInfo fn_7100084364(UnkHandle* self) {
    return self->target->vf_0x20();
}
