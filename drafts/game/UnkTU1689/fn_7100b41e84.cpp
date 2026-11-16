// GUI::CMapMenuController unit (1689): looks the string id at +0x4a8 up in the game manager's world
// (as fn_7100b41fac) and tells whether the selected entry's (+0x4b0) name is set and none of three
// reserved names. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

void* fn_7100e94428(void* world, base::global::CStrId* id);
extern CStrId g_unk_71727c8030;
extern CStrId g_unk_71727c8040;
extern CStrId g_unk_71727c8078;

// The entries are only 4-aligned, so the name pointer sits at +4.
typedef const CStrId* __attribute__((aligned(4))) UnkNamePtr_lb41e84;

struct UnkMapEntry_lb41e84 {
    u32 field_0x0;
    UnkNamePtr_lb41e84 name;
    u8 pad_0xc[0x578 - 0xc];
};
static_assert(sizeof(UnkMapEntry_lb41e84) == 0x578, "layout");

struct UnkMapMenuController_lb41e84 {
    u8 pad_0x0[0x4a8];
    CStrId id;
    u32 index;  // 0x4b0
};

static inline UnkMapEntry_lb41e84* Lookup(UnkMapMenuController_lb41e84* self) {
    CStrId* id = &self->id;
    return static_cast<UnkMapEntry_lb41e84*>(fn_7100e94428(g_unk_7101d47310->field_0x2228, id));
}

bool fn_7100b41e84(UnkMapMenuController_lb41e84* self) {
    if (self->id.data == &g_unk_7101d47108->empty)
        return false;
    UnkMapEntry_lb41e84* entries = Lookup(self);
    CStrId name = entries ? *entries[self->index].name : g_unk_7101d470e0;
    if (g_unk_71727c8030 == name || g_unk_71727c8040 == name || g_unk_71727c8078 == name)
        return false;
    if (name.data == &g_unk_7101d47108->empty)
        return false;
    return true;
}
