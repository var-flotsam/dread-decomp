// CEmmyProtoAIComponent's unit (tools/tu_map.py unit 791): fn_71004fc4a8, whether the grab tutorial
// is still to come: the inverse of the flag EMMY_PROTO_GRAB_TUTO_LAUNCHED (the property
// g_unk_71726dd158, its qualified id built on first use) in the section of the global manager's
// blackboard (+0x968) that the singleton's id at +0x1bd0 names (fn_7100df6d84); true without one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

// The singleton's section id (refl_GetSingletonPart, src/reflection_statics/statics_01.cpp).
void* fn_7100df6d84(UnkGameManager* gm);

extern UnkPropertyEx<> g_unk_71726dd158;

// The property as a blackboard definition (game/UnkGameBlackboard.h): its qualified id at +0x10.
struct UnkProtoNamedId_A3_b66 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};

bool fn_71004fc4a8() {
    bool flag = false;
    UnkGameManager* gm = g_unk_7101d47310;
    CBlackboard* blackboard = &gm->field_0x968;
    const CStrId& id = *static_cast<const CStrId*>(fn_7100df6d84(gm));
    UnkProtoNamedId_A3_b66& def = reinterpret_cast<UnkProtoNamedId_A3_b66&>(g_unk_71726dd158);
    if (def.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, id))
        fn_710006d11c(section, &def.qid.id, &flag);
    return !flag;
}
