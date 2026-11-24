// What each E.M.M.I. AI component does when its E.M.M.I. is destroyed, one function per area (the
// first in CEmmyCaveAIComponent's unit, the others next to it): a message (kind 0, the "defeated"
// text id, an empty list of argument ids) is sent to the game manager's callback set at +0x2128,
// then the scenario script's matching function is called. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkMlogMessage_b42a1.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::global::CStrId;

__attribute__((always_inline)) static inline void SendDefeated(const char* text, const char* script) {
    UnkMlogMessage_b42a1 message(CStrId(text, true));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128, &message);
    fn_7100000790(script, "");
}

void CEmmyCaveAIComponent_SendDefeatedMessageAndCallOnEmmyCaveDead() { SendDefeated("#MLOG_DEFEAT_EMMY_CAVE", "CurrentScenario.OnEmmyCaveDead"); }
void fn_71004f2d5c() { SendDefeated("#MLOG_DEFEAT_EMMY_FOREST", "CurrentScenario.OnEmmyForestDead"); }
void fn_71004f7a30() { SendDefeated("#MLOG_DEFEAT_EMMY_LAB", "CurrentScenario.OnEmmyBaseLabDead"); }
void fn_71004f934c() { SendDefeated("#MLOG_DEFEAT_EMMY_MAGMA", "CurrentScenario.OnEmmyMagmaDead"); }
void fn_71004ff94c() { SendDefeated("#MLOG_DEFEAT_EMMY_SANC", "CurrentScenario.OnEmmySancDead"); }
