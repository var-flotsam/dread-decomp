// CGoliathXBurstProjectionBombMovement's unit: creates the movement's two sets of 128 effect objects
// (+0x520 and +0x920) from the unit's two effect paths, puts each on render layer 0, scales it to half and
// switches it off. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAbilitySonar.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU285.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;

// The two effect paths the objects are created from.
__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_71726e5ce8[2];

struct UnkGoliathXBombFx_l58a4c0 {
    u8 pad_0x0[0x520];
    UnkSwitchObject* fx[2][128];  // 0x520: one set per effect path
};

// A switch object created from `path` by the render manager's pool and put on layer 0.
static inline UnkSwitchObject* CreateOnLayer0(const CFilePathStrId* path) {
    UnkRenderManager* render = g_unk_7101d47300;
    UnkSwitchObject* object = nullptr;
    fn_7100339d48(render->field_0x218, path, &object, &g_unk_7101d470e0);
    fn_7100219ae0(render, object ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<u8*>(object) + 8) : nullptr,
                  0);
    return object;
}

static inline void CreateHalfSizeOff(UnkSwitchObject** slot, const CFilePathStrId* path) {
    UnkSwitchObject* object = CreateOnLayer0(path);
    *slot = object;
    UnkVec3f scale(0.5f, 0.5f, 0.5f);
    fn_7100208fa4(reinterpret_cast<UnkXformNode*>(object->field_0x28), &scale);
    fn_7100146dac(*slot, false);
}

void fn_710058a4c0(UnkGoliathXBombFx_l58a4c0* self) {
    for (u32 i = 0; i < 128; ++i) {
        CreateHalfSizeOff(&self->fx[0][i], &g_unk_71726e5ce8[0]);
        CreateHalfSizeOff(&self->fx[1][i], &g_unk_71726e5ce8[1]);
    }
}
