// CPickable's unit (tools/tu_map.py unit 2591): the pickable's owner and name setter. With an owner,
// the owner's weak handle (+0xb0) and the given name (+0xb8) are taken; without one, both are
// cleared. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkPickable_ovb0083 {
    u8 pad_0x0[0xb0];
    UnkWeakHandle* owner;  // 0xb0
    CStrId name;           // 0xb8
};

void fn_7100f05a5c(UnkPickable_ovb0083* self, UnkWeakTarget* owner, const CStrId& name) {
    if (owner) {
        AssignRef(&self->owner, GetWeakHandle(owner));
        self->name = name;
    } else {
        ResetRef(&self->owner);
        self->name = g_unk_7101d470e0;
    }
}
