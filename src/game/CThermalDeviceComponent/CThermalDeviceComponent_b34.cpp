#include "base/global/CStrId.h"
#include "types.h"

struct CThermalDeviceComponent {
    u8 unk_0x0[0x590];
    base::global::UnkStringData* name;  // a CStrId's data
};

void* fn_7100a9f65c(CThermalDeviceComponent* self);

// While the id at +0x590 is still the empty string, defers to fn_7100a9f65c; else returns the id.
void* fn_7100a928c4(CThermalDeviceComponent* self) {
    if (self->name != &g_unk_7101d47108->empty)
        return &self->name;
    return fn_7100a9f65c(self);
}
