// CVulkranAttackManager::CTunableVulkranAttackManager's unit (tools/tu_map.py), fn_71006de940: drops the handle held
// at +0x8 when it refers to `target`, then hands a counted reference to `target`'s weak handle to the list at +0x18.
// Nothing happens for a null target. Names and layouts are placeholders.
#include "game/UnkTU2523.h"
#include "game/UnkTU852.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHandleList {
    void* field_0x0;
};

struct UnkVulkranAttackTargets_l6de940 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* field_0x8;
    u8 pad_0x10[8];
    UnkHandleList field_0x18;
};

static inline UnkRefPtr UnkVulkranMakeRef_l6de940(UnkWeakHandle* handle) {
    UnkRefPtr ref;
    ref.field_0x0 = nullptr;
    if (handle) {
        ++handle->ref_count;
        ref.field_0x0 = handle;
    }
    return ref;
}

void fn_71006de940(UnkVulkranAttackTargets_l6de940* self, UnkWeakTarget* target) {
    if (!target)
        return;
    UnkWeakHandle* held = self->field_0x8;
    if (held) {
        // keeps the second null test of the handle, as the original does
        asm("" : "+r"(held));
        if (held && held->target == target) {
            if (--held->ref_count == 0)
                held->vf_0x10();
            self->field_0x8 = nullptr;
        }
    }
    fn_710059cecc(&self->field_0x18, UnkVulkranMakeRef_l6de940(fn_7100d78378(target)));
}
