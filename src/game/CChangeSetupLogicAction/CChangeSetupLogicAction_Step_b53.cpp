// CChangeSetupLogicAction unit (tools/tu_map.py, the name from reflection): the action's step. After the
// base step (fn_7100a55ac8), unless its setup id (+0x10) is empty, hands the id and the flags at +0x18 and
// +0x19 to the manager g_unk_7101d47360 (created on first use, as in
// src/game/CBreakableVignetteComponent/CBreakableVignetteComponent_b40.cpp): through fn_7100eea3fc when
// the flag at +0x1a is set, else fn_7100eea334. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkTU2577.h"
#include "types.h"

struct UnkTutoExitAction_b47;

struct UnkManager;
extern UnkManager* g_unk_7101d47360;
void fn_7100eea3fc(void* manager, const base::global::CStrId* id, bool a, bool b);
void fn_7100eea334(void* manager, const base::global::CStrId* id, bool a, bool b);

struct UnkChangeSetupAction_b53 {
    void** vtable;
    u8 pad_0x8[0x8];
    base::global::CStrId id;  // 0x10
    bool field_0x18;
    bool field_0x19;
    bool field_0x1a;
};

void fn_7100a37690(UnkChangeSetupAction_b53* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    if (self->id.data == &g_unk_7101d47108->empty)
        return;
    bool mode = self->field_0x1a;
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    if (mode)
        fn_7100eea3fc(manager, &self->id, self->field_0x18, self->field_0x19);
    else
        fn_7100eea334(manager, &self->id, self->field_0x18, self->field_0x19);
}
