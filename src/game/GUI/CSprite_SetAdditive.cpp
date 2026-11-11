// GUI::CSprite unit: setting the sprite's additive-blend flag (+0x1e7) and, when it changed and the
// sprite has a render object (+0x190), the same flag on that object's blend layer, which then
// re-derives its blend mode (fn_710019c284, src/game/UnkUnit224/fn_710019db08.cpp). Names and
// layouts are placeholders.
#include "game/UnkTU223.h"
#include "types.h"

struct UnkBlendLayer_710019c284;

struct UnkSpriteBlendLayer {
    u8 pad_0x0[0x181];
    bool additive;  // 0x181
};

struct UnkSpriteRenderNode {
    u8 pad_0x0[0x78];
    UnkSpriteBlendLayer* layer;  // 0x78
};

struct UnkSpriteRenderObject {
    u8 pad_0x0[0x178];
    UnkSpriteRenderNode* node;  // 0x178
};

struct UnkSprite {
    u8 pad_0x0[0x190];
    UnkSpriteRenderObject* render;  // 0x190
    u8 pad_0x198[0x4f];
    bool additive;  // 0x1e7
};

void fn_71011fa404(UnkSprite* self, bool additive) {
    if (self->additive != additive) {
        self->additive = additive;
        if (UnkSpriteRenderObject* render = self->render) {
            UnkSpriteBlendLayer* layer = render->node->layer;
            layer->additive = additive;
            fn_710019c284(reinterpret_cast<UnkBlendLayer_710019c284*>(layer));
        }
    }
}
