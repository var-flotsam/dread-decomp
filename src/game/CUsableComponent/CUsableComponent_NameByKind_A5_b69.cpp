// CUsableComponent's unit: the id (a string id) that one of the component's virtual getters gives for
// a kind (1: slot 0x218, 3: 0x1f0, 4: 0x1f8, 5: 0x208, 6: 0x200, 7: 0x210), or the global empty id.
// Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CUsableComponent.h"

using base::global::CStrId;

struct UnkUsableComponent_A5_b69 {
    void** vtable;

    const CStrId& Get(u32 offset) {
        return reinterpret_cast<const CStrId& (*)(UnkUsableComponent_A5_b69*)>(vtable[offset / 8])(this);
    }
};

const CStrId& fn_7100aa1888(UnkUsableComponent_A5_b69* self, u32 kind) {
    switch (kind) {
    case 1:
        return self->Get(0x218);
    case 3:
        return self->Get(0x1f0);
    case 4:
        return self->Get(0x1f8);
    case 5:
        return self->Get(0x208);
    case 6:
        return self->Get(0x200);
    case 7:
        return self->Get(0x210);
    default:
        return g_unk_7101d470e0;
    }
}
