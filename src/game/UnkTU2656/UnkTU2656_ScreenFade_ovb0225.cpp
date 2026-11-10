// From unit 2656 of tools/tu_map.py (0x7101053650-0x7101053e24, no reliable name): the screen fade
// (src/game/UnkScreenFade.cpp) set-up. Names and
// layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkFadeTarget {
    u8 unk_0x0[0x18];
    u8 node[0x138];
    union {
        bool hidden;  // +0x150
    };
};

struct UnkScreenFade {
    UnkFadeTarget* target;
    bool enabled;  // +0x8
    bool armed;    // +0x9
    u8 unk_0xa[0x2];
    f32 r, g, b, a;  // 0xc
    f32 duration;    // 0x1c
    f32 delay;       // 0x20
    f32 elapsed;     // 0x24
    u32 field_0x28;
};
static_assert(__builtin_offsetof(UnkScreenFade, field_0x28) == 0x28, "UnkScreenFade placeholder layout");

void fn_7100e13abc(UnkRenderPool* pool, const CFilePathStrId* path, UnkFadeTarget** out, const CStrId* name);
void fn_710020a094(void* node, u32 layer);


// Takes the white texture's node, puts it on layer 2 and gives it the layer `layer`.
void fn_7101053650(UnkScreenFade* self, u32 layer, bool enabled) {
    self->enabled = enabled;
    self->armed = false;
    __builtin_memset(&self->r, 0, 0x20);
    UnkRenderManager* render = g_unk_7101d47300;
    UnkFadeTarget* node;
    {
        CFilePathStrId path(base::global::UnkStrIdInline(), "gui/textures/white.bctex", true);
        node = nullptr;
        fn_7100e13abc(render->field_0x218, &path, &node, &g_unk_7101d470e0);
        fn_7100219ae0(render, reinterpret_cast<UnkSceneNode*>(node), 2);
        self->target = node;
    }
    fn_710020a094(self->target->node, layer);
    self->field_0x28 = 1;
}
