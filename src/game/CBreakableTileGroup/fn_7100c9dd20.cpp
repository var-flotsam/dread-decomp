// CBreakableTileGroup's unit: the destructor of one 0xa8-byte breakable tile: removes its render
// object; its three id/path slots (last first) and its name id are then released as members.
// Names and layouts are placeholders.
#include "game/UnkBreakableTile.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

UnkBreakableTile_b49::~UnkBreakableTile_b49() {
    if (render_object != 0xffffffff) {
        fn_7100219e10(g_unk_7101d47300, render_object);
        render_object = 0xffffffff;
    }
}
