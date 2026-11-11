// The complete-object destructor beside fn_71006de820 (CVulkranAttackManager::CTunableVulkranAttackManager
// unit): restore the vtable, destroy the member at +0x18 (fn_710059c7b8) and drop the counted reference
// at +0x8. Names and layouts are placeholders.
#include "game/UnkTU852.h"
#include "game_s2/UnkWeakHandle.h"

extern void* g_unk_7101a69098[];

struct UnkObject_71006de7c4 {
    void** vtable;
    UnkRefObject* field_0x8;
    u64 field_0x10;
    u8 field_0x18[0x10];
};

void fn_71006de7c4(UnkObject_71006de7c4* self) {
    self->vtable = g_unk_7101a69098 + 2;
    fn_710059c7b8(self->field_0x18);
    if (UnkRefObject* p = self->field_0x8) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
}
