// sound::CMusicManager unit (tools/tu_map.py unit 2637): fn_7100fdca28 sets the flag at +0xf08; clearing
// it runs fn_7100fd613c (with 1) on every track (count at +0x250). Names and layouts are placeholders.
#include "types.h"

struct UnkMusicManager_b49 {
    u8 pad_0x0[0x250];
    u32 track_count;
    u8 pad_0x254[0xcb4];
    bool flag;  // 0xf08
};

void fn_7100fd613c(UnkMusicManager_b49* self, u32 track, bool value);

void fn_7100fdca28(UnkMusicManager_b49* self, bool flag) {
    if (self->flag == flag)
        return;
    self->flag = flag;
    if (!flag) {
        for (u32 i = 0; i < self->track_count; ++i)
            fn_7100fd613c(self, i, true);
    }
}
