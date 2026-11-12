// CMeleeComponent unit (unit 1421 of tools/tu_map.py): drops the current melee target: fn_7100929298
// with false, clears the flag at +0x98, unregisters the link at +0xa8 from the target the weak handle
// at +0xc0 points to and releases the handle, resets the string id at +0xd8 and clears +0x164.
// Names and layout are placeholders.
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkMeleeTarget_b53 {
    u8 pad_0x0[0x98];
    bool field_0x98;
    u8 pad_0x99[0xf];
    u8 field_0xa8[0x18];  // the link registered with the target
    UnkWeakHandle* field_0xc0;
    u8 pad_0xc8[0x10];
    UnkStringData* field_0xd8;  // CStrId
    u8 pad_0xe0[0x84];
    u32 field_0x164;
};

void fn_7100929298(UnkMeleeTarget_b53* self, u32);

void fn_710092b548(UnkMeleeTarget_b53* self) {
    fn_7100929298(self, false);
    self->field_0x98 = false;
    if (UnkWeakHandle* handle = self->field_0xc0) {
        if (handle->target)
            fn_710117b3f0(handle->target, self->field_0xa8);
        ResetRef(&self->field_0xc0);
    }
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->field_0xd8;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
    self->field_0x164 = 0;
}
