// Unit 2571: a missile count from the game manager's blackboards: two per missile tank and ten per
// missile plus tank collected, ten more once the archive flag g_unk_717275b148 is set, plus 30 on
// difficulty 3 and 15 otherwise. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU500.h"
#include "types.h"

using base::global::CStrId;

static inline const CStrId& Section_lec74c8(u32 index) {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[index];
}

static inline const UnkQualifiedId& DefId_lec74c8(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), g_unk_7101d470e0);
    return qid;
}

// Reads the number `def` names in the blackboard section `section` into `out`, when the section exists.
static inline void ReadNumber_lec74c8(CBlackboard* blackboard, const CStrId& section, UnkPropertyEx<>& def,
                                      f32* out) {
    const UnkQualifiedId& name = DefId_lec74c8(def);
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_7100395b34(s, &name.id, out);
}

// Reads the flag `def` names in the blackboard section `section` into `out`, when the section exists.
static inline void ReadFlag_lec74c8(CBlackboard* blackboard, const CStrId& section, UnkPropertyEx<>& def,
                                    bool* out) {
    const UnkQualifiedId& name = DefId_lec74c8(def);
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_710006d11c(s, &name.id, out);
}

u32 fn_7100ec74c8(u8* self) {
    CBlackboard* game = &g_unk_7101d47310->field_0x968;
    f32 tanks = 0.0f;
    ReadNumber_lec74c8(game, Section_lec74c8(0x37b), g_unk_717275a358, &tanks);
    f32 plus_tanks = 0.0f;
    ReadNumber_lec74c8(game, Section_lec74c8(0x37b), g_unk_717275a398, &plus_tanks);
    f32 total = tanks * 2.0f + plus_tanks * 10.0f;
    CBlackboard* archive = &g_unk_7101d47310->field_0x820;
    u32 count = total;

    bool upgraded = false;
    ReadFlag_lec74c8(archive, Section_lec74c8(0xb4), g_unk_717275b148, &upgraded);
    if (upgraded)
        count += 10;
    return (fn_7100dffc58(g_unk_7101d47310) == 3 ? 30 : 15) + count;
}
