// Functions in the CSamusGunComponent unit (tools/tu_map.py). Names and layouts are placeholders.
#include "types.h"

struct UnkQueryObj;
u32 fn_7100c9e960(UnkQueryObj* obj);
UnkQueryObj* fn_7100ca2048(void* where, void* key, u32 flags);
struct UnkBreakableTile;
void fn_7100c9fe00(UnkBreakableTile* tile, s32 kind, s32 mode, bool flag, bool force);

struct UnkSamusGunComponent {
    virtual void vf_0x0();
    bool vf_0x1c8(u32 kind) {
        return reinterpret_cast<bool (*)(UnkSamusGunComponent*, u32)>(
            (*reinterpret_cast<void***>(this))[0x1c8 / 8])(this, kind);
    }
};

// Looks up the object at `key` (+0x14 of `info`) and, when this component accepts its kind,
// switches it (fn_7100c9fe00). Returns whether it did.
bool fn_710085aab0(UnkSamusGunComponent* self, void* where, char* info) {
    UnkQueryObj* obj = fn_7100ca2048(where, info + 0x14, 1);
    if (obj && self->vf_0x1c8(fn_7100c9e960(obj))) {
        fn_7100c9fe00(reinterpret_cast<UnkBreakableTile*>(obj), 0, 0, true, false);
        return true;
    }
    return false;
}
