// CEmmyShipyardAIComponent's unit (tools/tu_map.py): what the component does when its E.M.M.I. is destroyed, as
// src/game/CEmmyCaveAIComponent/UnkEmmyDefeatedMessage_b42.cpp: the "defeated" message to the game manager's callback set,
// the component's own handler (fn_7100554740), then the scenario script's OnEmmyShipyardDead. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
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

void fn_7100456b7c(void* callbacks, const UnkMlogMessage_b42a1* message);
void fn_7100000790(const char* name, const char* format, ...);  // calls a script function


void fn_7100554740(void* self, void* arg);

void fn_7100504534(void* self, void* arg) {
    UnkMlogMessage_b42a1 message(CStrId("#MLOG_DEFEAT_EMMY_SHIP", true));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128, &message);
    fn_7100554740(self, arg);
    fn_7100000790("CurrentScenario.OnEmmyShipyardDead", "");
}
