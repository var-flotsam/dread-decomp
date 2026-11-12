// CVulkranAttackManager::CTunableVulkranAttackManager's unit (tools/tu_map.py), fn_71006ded20: hands a
// counted reference to `target`'s weak handle (fn_7100d78378) to the list at +0x18 (fn_710059cecc, as
// fn_71006bd128 in src/game_lo/fn_71006bd128.cpp does), then drops the handle held at +0x8 when it refers
// to `target`, and reloads the word at +0x10 from the tunable's +0x24. Nothing happens for a null target.
// Names and layouts are placeholders.
#include "game/UnkTU2523.h"
#include "game/UnkTU852.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHandleList {
    void* field_0x0;
};

extern UnkTunable g_unk_717270a770;

struct UnkVulkranAttackTargets_A3_b75 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* field_0x8;
    u32 field_0x10;
    u8 pad_0x14[4];
    UnkHandleList field_0x18;
};

static inline UnkRefPtr UnkVulkranMakeRef_A3_b75(UnkWeakHandle* handle) {
    UnkRefPtr ref;
    ref.field_0x0 = nullptr;
    if (handle) {
        ++handle->ref_count;
        ref.field_0x0 = handle;
    }
    return ref;
}

void fn_71006ded20(UnkVulkranAttackTargets_A3_b75* self, UnkWeakTarget* target) {
    if (!target)
        return;
    fn_710059cecc(&self->field_0x18, UnkVulkranMakeRef_A3_b75(fn_7100d78378(target)));
    UnkWeakHandle* held = self->field_0x8;
    if ((held ? held->target : nullptr) != target)
        return;
    ResetRef(&self->field_0x8);
    self->field_0x10 = *reinterpret_cast<u32*>(reinterpret_cast<char*>(&g_unk_717270a770) + 0x24);
}
