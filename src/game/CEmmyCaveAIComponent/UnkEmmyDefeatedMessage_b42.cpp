// What each E.M.M.I. AI component does when its E.M.M.I. is destroyed, one function per area (the
// first in CEmmyCaveAIComponent's unit, the others next to it): a message (kind 0, the "defeated"
// text id, an empty list of argument ids) is sent to the game manager's callback set at +0x2128,
// then the scenario script's matching function is called. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::global::CStrId;

// The message's text id: copied like a CStrId, released in two steps like base::UnkStrIdMember.
struct UnkMlogText_b42a1 : base::global::UnkStrIdBase {
    __attribute__((always_inline)) UnkMlogText_b42a1(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    __attribute__((always_inline)) ~UnkMlogText_b42a1() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkMlogMessage_b42a1 {
    u32 kind;
    UnkMlogText_b42a1 text;
    base::UnkVector<base::UnkStrIdMember> args;

    __attribute__((always_inline)) UnkMlogMessage_b42a1(const CStrId& t) : kind(0), text(t), args(0xc) {}
    __attribute__((always_inline)) ~UnkMlogMessage_b42a1() {
        args.Free();
    }
};

__attribute__((always_inline)) static inline void SendDefeated(const char* text, const char* script) {
    UnkMlogMessage_b42a1 message(CStrId(text, true));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128, &message);
    fn_7100000790(script, "");
}

void fn_71004f0ab8() { SendDefeated("#MLOG_DEFEAT_EMMY_CAVE", "CurrentScenario.OnEmmyCaveDead"); }
void fn_71004f2d5c() { SendDefeated("#MLOG_DEFEAT_EMMY_FOREST", "CurrentScenario.OnEmmyForestDead"); }
void fn_71004f7a30() { SendDefeated("#MLOG_DEFEAT_EMMY_LAB", "CurrentScenario.OnEmmyBaseLabDead"); }
void fn_71004f934c() { SendDefeated("#MLOG_DEFEAT_EMMY_MAGMA", "CurrentScenario.OnEmmyMagmaDead"); }
void fn_71004ff94c() { SendDefeated("#MLOG_DEFEAT_EMMY_SANC", "CurrentScenario.OnEmmySancDead"); }
