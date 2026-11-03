// GUI::CMapMenuController unit (tools/tu_map.py): looks the string id at +0x4a8 up in the world held by the
// game manager (fn_7100e94428) unless it is the empty id. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

void* fn_7100e94428(void* world, base::global::CStrId* id);

struct UnkMapMenuController_b47a3 {
    u8 unk_0x0[0x4a8];
    base::global::CStrId id;
};

void* fn_7100b41fac(UnkMapMenuController_b47a3* self) {
    if (self->id.data == &g_unk_7101d47108->empty)
        return nullptr;
    return fn_7100e94428(g_unk_7101d47310->field_0x2228, &self->id);
}
