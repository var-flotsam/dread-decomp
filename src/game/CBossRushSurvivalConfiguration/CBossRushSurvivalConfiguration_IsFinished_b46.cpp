// CBossRushSurvivalConfiguration's unit (tools/tu_map.py unit 2544): whether the run is over: in
// one mode (+0xbb) when the last stage (12) is reached and the progress (+0xd0) has got to twelve
// times the tunable's count (+0x34) less one; otherwise, unless the flag at +0xba is set, when the
// last stage is reached. Names and layouts are placeholders.
#include "game/UnkTU2544.h"
#include "types.h"

struct UnkTunable_71727bd078;

struct UnkSurvivalState_b46a3 {
    u8 pad_0x0[0xba];
    bool field_0xba;
    bool field_0xbb;
    u8 pad_0xbc[0x14];
    s32 progress;  // 0xd0
    u8 pad_0xd4[0x2c];
    s32 stage;  // 0x100
};

bool fn_7100dca1bc(UnkSurvivalState_b46a3* self) {
    if (self->field_0xbb) {
        if (self->stage != 12)
            return false;
        s32 progress = self->progress;
        return progress >= *reinterpret_cast<s32*>(reinterpret_cast<char*>(&g_unk_71727bd078) + 0x34) * 12 - 1;
    }
    if (self->field_0xba)
        return false;
    return self->stage == 12;
}
