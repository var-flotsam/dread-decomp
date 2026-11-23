// Unit 2200 (tools/tu_map.py; CCutSceneDef neighbourhood): fn_7100c886c0 is an element's destructor:
// releases its string ids (two of them file-path ids), clears and frees its table at +0x70 and destroys
// its keyed ids at +0x28, last member first. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkKeyedStrIds.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkCutSceneTable_lc886c0.h"
#include "game/UnkTU2200.h"
#include "types.h"

using UnkPathIdMember_lc886c0 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

struct UnkCutSceneEntry_lc886c0 {
    base::UnkStrIdMember field_0x0;
    UnkPathIdMember_lc886c0 field_0x8;
    base::UnkStrIdMember field_0x10;
    base::UnkStrIdMember field_0x18;
    u8 pad_0x20[0x8];
    UnkKeyedStrIds field_0x28;
    u8 pad_0x50[0x8];
    base::UnkStrIdMember field_0x58;
    base::UnkStrIdMember field_0x60;
    u8 pad_0x68[0x8];
    UnkCutSceneTable_lc886c0 field_0x70;
    UnkPathIdMember_lc886c0 field_0x98;
};
static_assert(__builtin_offsetof(UnkCutSceneEntry_lc886c0, field_0x70) == 0x70, "layout");
static_assert(__builtin_offsetof(UnkCutSceneEntry_lc886c0, field_0x98) == 0x98, "layout");

void fn_7100c886c0(void* p) {
    auto* self = static_cast<UnkCutSceneEntry_lc886c0*>(p);
    self->field_0x98.~UnkStrIdMemberOf();
    UnkCutSceneTable_lc886c0* table = &self->field_0x70;
    fn_7100c88920(table);
    if (table->field_0x8)
        table->allocator->vf_0x48(table->field_0x8);
    if (table->field_0x0)
        table->allocator->vf_0x48(table->field_0x0);
    self->field_0x60.~UnkStrIdMemberOf();
    self->field_0x58.~UnkStrIdMemberOf();
    fn_71000df388(&self->field_0x28);
    self->field_0x18.~UnkStrIdMemberOf();
    self->field_0x10.~UnkStrIdMemberOf();
    self->field_0x8.~UnkStrIdMemberOf();
    self->field_0x0.~UnkStrIdMemberOf();
}
