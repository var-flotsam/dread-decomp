// A deleting destructor in the CVulkranAttackManager::CTunableVulkranAttackManager unit (copy of
// fn_71006bd00c): restore the vtable, destroy the member at +0x18 (fn_710059c7b8), drop the counted
// reference at +0x8 and free the object. Names and layouts are placeholders.
#include "game/CVulkranAttackManager/CTunableVulkranAttackManager.h"
#include "game/UnkTU852.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkObject_71006de820 {
    void** vtable;
    UnkRefObject* field_0x8;
    u64 field_0x10;
    u8 field_0x18[0x10];
};

void fn_71006de820(UnkObject_71006de820* self) {
    self->vtable = g_unk_7101a69098 + 2;
    fn_710059c7b8(self->field_0x18);
    if (UnkRefObject* p = self->field_0x8) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    operator delete(self);
}
