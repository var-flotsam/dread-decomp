// CEmmyShipyardAIComponent's unit (tools/tu_map.py): what the component does when its E.M.M.I. is destroyed, as
// src/game/CEmmyCaveAIComponent/UnkEmmyDefeatedMessage_b42.cpp: the "defeated" message to the game manager's callback set,
// the component's own handler (fn_7100554740), then the scenario script's OnEmmyShipyardDead. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkMlogMessage_b42a1.h"
#include "game/UnkTU0.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CStrId;

void fn_7100504534(void* self, void* arg) {
    UnkMlogMessage_b42a1 message(CStrId("#MLOG_DEFEAT_EMMY_SHIP", true));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128, &message);
    fn_7100554740(self, arg);
    fn_7100000790("CurrentScenario.OnEmmyShipyardDead", "");
}
