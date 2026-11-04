// CEmmySancAIComponent's unit (tools/tu_map.py): hands the unit's name g_unk_71726dd8f8 and 0 to the
// AI manager g_unk_71726af468 (created on first use) through fn_7100daa92c. Names are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

using base::global::CStrId;

void fn_7100daa92c(UnkManager_71726af468* self, CStrId name, u32 value);
extern CStrId g_unk_71726dd8f8 __attribute__((visibility("hidden")));

void fn_71004ffe18() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    fn_7100daa92c(m, CStrId(g_unk_71726dd8f8), 0);
}
