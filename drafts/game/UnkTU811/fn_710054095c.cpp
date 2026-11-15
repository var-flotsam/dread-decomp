// Unit 811 (CEmmyAIComponent hint): hands the game manager the string id for the E.M.M.I.'s barrier state:
// the barrier or no-barrier name when the flag at +0x14c35 is set (by the state at +0x27720), else the
// default id. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkEmmyAI2_b57a3 {
    u8 pad_0x0[0x14c35];
    u8 barrierFlag;  // 0x14c35
    u8 pad_0x14c36[0x12aea];
    u8 barrierUp;  // 0x27720
};

void fn_710054095c(UnkEmmyAI2_b57a3* self) {
    static base::global::CStrId s_barrier("emmy_barrier", true);
    static base::global::CStrId s_noBarrier("emmy_no_barrier", true);
    UnkGameManager* manager = g_unk_7101d47310;
    if (self->barrierFlag)
        fn_7100df5cec(manager, self->barrierUp == 0 ? &s_noBarrier : &s_barrier);
    else
        fn_7100df5cec(manager, &g_unk_7101d470e0);
}
