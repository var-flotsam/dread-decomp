// CTunableMusicManager (tools/tu_map.py unit 2637). fn_7100fdb9e0: starts the fade at +0x2b8 over `time`
// when the track at +0x10 is in state 3 or a restart is asked for (which clears the hold flag at +0xb4),
// unless that flag holds it. Names and layouts are placeholders.
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkMusicTrack_lfdb9e0 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkMusicFade_lfdb9e0;

struct UnkMgr_0xf18 {
    u8 pad_0x0[0x10];
    UnkMusicTrack_lfdb9e0* track;  // 0x10
    u8 pad_0x18[0x9c];
    bool held;  // 0xb4
    u8 pad_0xb5[0x203];
    UnkMusicFade_lfdb9e0* Fade() { return reinterpret_cast<UnkMusicFade_lfdb9e0*>(pad_0x2b8); }
    u8 pad_0x2b8[1];
};

void fn_7100fd0d40(UnkMusicFade_lfdb9e0* fade, f32 time);

void fn_7100fdb9e0(UnkMgr_0xf18* manager, u32 kind, u32 flag, f32 a, f32 b) {
    bool restart = flag & 1;
    bool playing = manager->track && manager->track->state == 3;
    if (restart)
        manager->held = false;
    if ((playing | restart) & !manager->held)
        fn_7100fd0d40(manager->Fade(), b);
}
