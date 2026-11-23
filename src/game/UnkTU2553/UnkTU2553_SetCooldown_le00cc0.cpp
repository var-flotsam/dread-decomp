// Unit 2553 (CGameManager::CTunableGameManager): sets the cooldown flag unless the blackboard says the
// cooldown has finished, tells its watchers and stores it in the blackboard. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU39.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

typedef base::UnkDeferredSet<base::UnkDeferredCallback<bool>> UnkFlagWatchers_ldf2b68;

struct UnkCooldownManager_ldf2b68 {
    u8 pad_0x0[0x968];
    CBlackboard blackboard;  // 0x968
    u8 pad_0xa58[0x13d0];
    UnkFlagWatchers_ldf2b68 cooldown_watchers;  // 0x1e28
    UnkFlagWatchers_ldf2b68 field_0x1ea8;       // 0x1ea8
    u8 pad_0x1f28[0x519];
    bool cooldown;      // 0x2441
    bool field_0x2442;  // 0x2442
};
static_assert(__builtin_offsetof(UnkCooldownManager_ldf2b68, cooldown_watchers) == 0x1e28, "layout");
static_assert(__builtin_offsetof(UnkCooldownManager_ldf2b68, cooldown) == 0x2441, "layout");

static inline const UnkQualifiedId& DefId_le00cc0(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), base::global::CStrId::Empty());
    return qid;
}

// Reads the flag `def` names in the blackboard section `section` into `out`, when the section exists.
static inline void ReadFlag_le00cc0(CBlackboard* blackboard, const CStrId& section, UnkPropertyEx<>& def,
                                    bool* out) {
    const UnkQualifiedId& name = DefId_le00cc0(def);
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_710006d11c(s, &name.id, out);
}

// Stores the flag `value` points to under `name` in the section `section`, unless the blackboard is locked.
static inline void SetFlag_le00cc0(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                                   bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

void fn_7100e00cc0(UnkGameManager* manager, bool on) {
    auto* self = reinterpret_cast<UnkCooldownManager_ldf2b68*>(manager);
    if (self->cooldown == on)
        return;
    bool finished = false;
    CBlackboard* blackboard = &self->blackboard;
    ReadFlag_le00cc0(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229],
                     g_unk_7172759278, &finished);
    if (finished)
        return;
    self->cooldown = on;
    fn_71000303c0(&self->cooldown_watchers, &self->cooldown);
    SetFlag_le00cc0(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229],
                    DefId_le00cc0(g_unk_7172759238), &self->cooldown);
    if (!on) {
        const CStrId& section = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229];
        const UnkQualifiedId& name = DefId_le00cc0(g_unk_7172759278);
        bool done = true;
        SetFlag_le00cc0(blackboard, section, name, &done);
    }
}
