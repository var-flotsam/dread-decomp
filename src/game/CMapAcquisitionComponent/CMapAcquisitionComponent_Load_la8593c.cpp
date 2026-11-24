// CMapAcquisitionComponent's unit: reads the "acquired" flag (+0x539) from the blackboard after the base
// method: first under the qualified id of the property g_unk_7172750dd0 in the section the component names
// (its slot 0x2d8), and when that is missing under the property g_unk_7172750da8's entry in its id table
// (+0x7d0). Names and layouts are placeholders.
#include "base/UnkStrIdTable.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CMapAcquisitionComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkMapAcquisition_la8593c {
    void** vtable;
    u8 pad_0x8[0x539 - 0x8];
    bool acquired;  // 0x539
    u8 pad_0x53a[0x7d0 - 0x53a];
    UnkStrIdTable table;  // 0x7d0

    // The id of the blackboard section the component's state is kept in.
    CStrId vf_0x2d8() { return reinterpret_cast<CStrId (*)(UnkMapAcquisition_la8593c*)>(vtable[0x2d8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkMapAcquisition_la8593c, table) == 0x7d0, "layout");

void fn_7100a9eff4(UnkMapAcquisition_la8593c* self, CBlackboard* blackboard);  // the base method

static inline const UnkQualifiedId& AcquiredId() {
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_7172750dd0);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(g_unk_7172750dd0), g_unk_7101d470e0);
    return qid;
}

// The value table's entry for `name` (the slot past the last one when it has none).
static inline UnkStrIdTableValue* FindValue(UnkStrIdTable* table, const UnkPropertyName& name) {
    u64 index = 0;
    u32 count = table->count;
    if (count) {
        CStrId* keys = table->keys;
        for (; index != count; ++index) {
            if (keys[index].data == name.data)
                break;
        }
    }
    return &table->values[index];
}

// Reads the flag `name` of the section `section_id` into `out`; false when either is missing.
static inline bool ReadFlag(CBlackboard* blackboard, const CStrId& section_id, const CStrId* name, bool* out) {
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        return fn_710006d11c(section, name, out);
    return false;
}

void CMapAcquisitionComponent_LoadFromBlackboard(UnkMapAcquisition_la8593c* self, CBlackboard* blackboard) {
    fn_7100a9eff4(self, blackboard);
    if (ReadFlag(blackboard, self->vf_0x2d8(), &AcquiredId().id, &self->acquired))
        return;
    ReadFlag(blackboard, self->vf_0x2d8(), &FindValue(&self->table, g_unk_7172750da8.name)->id, &self->acquired);
}
