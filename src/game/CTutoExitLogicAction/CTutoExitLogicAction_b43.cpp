// CTutoExitLogicAction's unit (tools/tu_map.py): its constructor, a copy of fn_7100f183a4
// (src/game/CStartCentralUnitCombatLogicAction/CStartCentralUnitCombatLogicAction_b42.cpp), on the logic-action
// base (fn_7100a55aa4): the vtable, then a member object at +0x10 built through its base (a vtable,
// an empty array with allocator flags 0xf and a cleared word) before its own vtable and a cleared
// pointer. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CTriggerLogicAction.h"
#include "types.h"

// The base constructor, as src/game/CChangeSetupLogicAction/CChangeSetupLogicAction_b35.cpp declares it.
struct UnkChangeSetupLogicActionDef;
extern char g_unk_7101a8f558[];
extern void* g_unk_7101a20998[];  // the member's base vtable
extern void* g_unk_7101a33fa0[];  // the member's vtable

struct UnkCombatLogicElem_b43a2;

struct UnkCombatLogicMember_b43a2 {
    void* vtable;
    base::UnkVector<UnkCombatLogicElem_b43a2*> items;
    u32 field_0x30;
    void* field_0x38;
};

struct UnkTutoExitLogicAction_b43a2 {
    void* vtable;
    u8 unk_0x8[0x8];
    UnkCombatLogicMember_b43a2 member;
};

void fn_7100a598d8(UnkTutoExitLogicAction_b43a2* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101a8f558 + 0x10;
    self->member.vtable = g_unk_7101a20998 + 2;
    new (&self->member.items) base::UnkVector<UnkCombatLogicElem_b43a2*>(0xf);
    self->member.field_0x30 = 0;
    self->member.vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
    self->member.field_0x38 = nullptr;
}
