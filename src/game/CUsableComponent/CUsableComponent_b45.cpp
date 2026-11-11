// CUsableComponent's unit (tools/tu_map.py unit 1607): the string id for a kind, from one of five
// virtuals (kind 1: slot 0x1b8; 4 to 7: slots 0x198 to 0x1b0), else the empty id
// (g_unk_7101d470e0). Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CUsableComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkUsableComponent_b45a2 {
    void** vtable;
    const CStrId& Call(u32 offset) {
        return reinterpret_cast<const CStrId& (*)(UnkUsableComponent_b45a2*)>(vtable[offset / 8])(this);
    }
};

const CStrId& fn_7100aa1f50(UnkUsableComponent_b45a2* self, u32 kind) {
    switch (kind) {
    case 1:
        return self->Call(0x1b8);
    case 4:
        return self->Call(0x198);
    case 5:
        return self->Call(0x1a0);
    case 6:
        return self->Call(0x1a8);
    case 7:
        return self->Call(0x1b0);
    default:
        return g_unk_7101d470e0;
    }
}
