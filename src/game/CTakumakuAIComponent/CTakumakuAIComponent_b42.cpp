// CTakumakuAIComponent unit (tools/tu_map.py, reflection hint). fn_71006d58f4: whether an id is one
// of the two the unit keeps (g_unk_7172709bc8, two words) or names the string whose hash
// (+0x30) is 0x893ccc073b5acb20. Names and layouts are placeholders.
#include "types.h"

struct UnkIdData_b42 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

// Two ids side by side (+0x8 is the second).
extern __attribute__((visibility("hidden"))) UnkIdData_b42* g_unk_7172709bc8[2];

bool fn_71006d58f4(void* self, UnkIdData_b42* const* id) {
    UnkIdData_b42* data = *id;
    if (data == g_unk_7172709bc8[1] || data == g_unk_7172709bc8[0])
        return true;
    return data->hash == 0x893ccc073b5acb20ull;
}
