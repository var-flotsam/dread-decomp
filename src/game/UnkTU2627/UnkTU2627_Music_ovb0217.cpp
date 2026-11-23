// Unit 2627 (tools/tu_map.py): a callback that starts the music manager's playback (fn_7100fdb9e0)
// while its state is 7. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/UnkMusicMgr_b0217.h"
#include "game/UnkMusicState_b0217.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkMgr_0xf18;

void fn_7100fce7dc() {
    if (!g_unk_71726bc560) {
        UnkMgr_0xf18* created = static_cast<UnkMgr_0xf18*>(fn_71000839a0(0xf18, 0x3f));
        fn_7100fd3804(created);
        g_unk_71726bc560 = created;
    }
    UnkMgr_0xf18* manager = g_unk_71726bc560;
    UnkMusicMgr_b0217* mgr = reinterpret_cast<UnkMusicMgr_b0217*>(manager);
    UnkMusicState_b0217* s = mgr->field_0x2f8;
    if (s && s->state == 7) {
        s32 override_state = mgr->field_0x38c;
        if (override_state == 0x7fffffff || override_state == 7)
            fn_7100fdb9e0(manager, 1, 0, -1.0f, 0.5f);
    }
}
