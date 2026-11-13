// CHeatRoomComponent unit (tools/tu_map.py): fn_7100a271e0 switches the heat room on or off. The
// activatable base stores the state first (fn_7100878680, src/game/CCharClassActivatableComponent/
// CCharClassActivatableComponent_b48.cpp); then, when the component may write (fn_710024fb2c) and the
// blackboard is not locked, the state is also stored under the owner's name (+0x30 of +0x30) within a
// named definition (built on first use, as src/game/CMinimapManager/CMinimapManager_ReadFlag_b43.cpp's
// SubId) in the current scenario's section, and the room is refreshed (fn_7100a26828). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CHeatRoomComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

// As src/game/UnkSubareaSetupState.cpp and src/game_s7/fn_7100fc4b34.cpp declare it.
struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);

void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// The activatable base, as CCharClassActivatableComponent_b48.cpp defines it.
struct UnkActivatable_b48;
void fn_7100878680(UnkActivatable_b48* self, bool enabled);

struct UnkHeatRoomNamedDef_b49 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
extern UnkHeatRoomNamedDef_b49 g_unk_7172746208;

static inline UnkSubId_7100fc4b34 SubId(UnkHeatRoomNamedDef_b49& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

struct UnkHeatRoomOwner_b49 {
    u8 pad_0x0[0x30];
    CStrId name;
};

struct UnkHeatRoom_b49 {
    u8 pad_0x0[0x30];
    UnkHeatRoomOwner_b49* owner;
};

void fn_7100a271e0(UnkHeatRoom_b49* self, bool on) {
    bool value = on;
    fn_7100878680((UnkActivatable_b48*)self, on);
    UnkGameManager* manager = g_unk_7101d47310;
    if (!fn_710024fb2c((UnkActivatable_b48*)self) || manager->field_0x968.field_0xe8)
        return;
    CBlackboard* blackboard = &manager->field_0x968;
    UnkTypedValue v;
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    {
        UnkSubId_7100fc4b34 key = SubId(g_unk_7172746208, self->owner->name);
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
            if (!blackboard->field_0xe8) {
                v.type = fn_710000d228();
                v.value = &value;
                fn_7100332c80(s, (UnkPropertyName*)&key.id, &v);
            }
        }
    }
    fn_7100a26828(self);
}
