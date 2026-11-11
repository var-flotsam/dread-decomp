// behaviortree::CSwitchBetweenAIModesAction's unit (tools/tu_map.py, unit 449): the node's destructor. Its vtable
// (0x7101a24aa8), the listener at +0x98 (vtable 0x7101a22d48) unlinked from the watched object behind the
// weak handle at +0xb0 and the handle released, the string ids at +0x70, +0x60, +0x58, +0x40, +0x38 and +0x30
// released, then the base's destructor fn_7100393ea0. Names are placeholders.
#include "base/UnkDestructors.h"
#include "game/behaviortree/CBaseAction.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a24aa8[];  // the node's vtable
extern void* g_unk_7101a22d48[];  // the listener's vtable

struct UnkSwitchAIModesDtor_ovb0092;
template <>
struct base::UnkDtorTraits<UnkSwitchAIModesDtor_ovb0092> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a24aa8); }
    static void base(char* self) { fn_7100393ea0(self); }
};

template <>
struct base::UnkLinkTraits<UnkSwitchAIModesDtor_ovb0092> {
    static char* link_vtable() { return reinterpret_cast<char*>(g_unk_7101a22d48); }
    static void unlink(char* list, char* link) { fn_710117b3f0(reinterpret_cast<UnkWeakTarget*>(list), link); }
};

struct UnkSwitchAIModesLink_ovb0092 {
    static void Release(char* self) { base::refl_ReleaseLink<UnkSwitchAIModesDtor_ovb0092, 0x98>(self); }
};

__attribute__((flatten)) void fn_710036be88(char* self) {
    base::refl_Dtor<UnkSwitchAIModesDtor_ovb0092, UnkSwitchAIModesLink_ovb0092, base::UnkStrIdAt<0x70>,
                    base::UnkStrIdAt<0x60>, base::UnkStrIdAt<0x58>, base::UnkStrIdAt<0x40>, base::UnkStrIdAt<0x38>,
                    base::UnkStrIdAt<0x30>>(self);
}
