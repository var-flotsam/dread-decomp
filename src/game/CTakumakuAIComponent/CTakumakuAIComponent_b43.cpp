// CTakumakuAIComponent unit (tools/tu_map.py, reflection hint). fn_71006d58b0: whether an id is one
// of the two the unit keeps in g_unk_7172709ba8 (two words) or names the string whose hash (+0x30) is
// 0x90f9b2366c1877a5; the shape of fn_71006d58f4
// (src/game/CTakumakuAIComponent/CTakumakuAIComponent_b42.cpp). Names and layouts are placeholders.
#include "types.h"

struct UnkIdData_b43a2 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

// Two ids side by side (+0x8 is the second).
extern __attribute__((visibility("hidden"))) UnkIdData_b43a2* g_unk_7172709ba8[2];

bool fn_71006d58b0(void* self, UnkIdData_b43a2* const* id) {
    UnkIdData_b43a2* data = *id;
    if (data == g_unk_7172709ba8[1] || data == g_unk_7172709ba8[0])
        return true;
    return data->hash == 0x90f9b2366c1877a5ull;
}
