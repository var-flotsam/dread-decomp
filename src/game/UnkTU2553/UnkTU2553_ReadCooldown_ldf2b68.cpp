// Unit 2553 (CGameManager::CTunableGameManager): reads the cooldown flags from the game manager's blackboard
// and hands each one to the callback set that watches it. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU39.h"
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

// Reads the flag `def` names in the blackboard section `section` into `out`, when the section exists.
static inline void ReadFlag_ldf2b68(CBlackboard* blackboard, const CStrId& section, UnkPropertyEx<>& def,
                                    bool* out) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), g_unk_7101d470e0);
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_710006d11c(s, &qid.id, out);
}

void fn_7100df2b68(void* manager) {
    auto* self = static_cast<UnkCooldownManager_ldf2b68*>(manager);
    self->cooldown = false;
    bool finished = false;
    CBlackboard* blackboard = &self->blackboard;
    ReadFlag_ldf2b68(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229],
                     g_unk_7172759278, &finished);
    if (!finished) {
        ReadFlag_ldf2b68(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229],
                         g_unk_7172759238, &self->cooldown);
        fn_71000303c0(&self->cooldown_watchers, &self->cooldown);
    }
    self->field_0x2442 = false;
    ReadFlag_ldf2b68(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x22a],
                     g_unk_7172759178, &self->field_0x2442);
    fn_71000303c0(&self->field_0x1ea8, &self->field_0x2442);
}
