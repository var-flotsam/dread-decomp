#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "engine/scene/UnkToggle.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

extern u32 g_unk_71726b21d4 __attribute__((visibility("hidden")));  // live pairs

struct UnkTunableBlock_ovb0259 {
    u8 pad_0x0[0x28];
    bool field_0x28;
};
extern UnkTunableBlock_ovb0259 g_unk_71727bc558;

struct UnkMinimapVec3;
struct UnkStruct_710020a5a0;
void fn_710019c564(void* target, f32 a, f32 b, f32 c, f32 d);
void fn_710020a4c4(void* p, const UnkMinimapVec3& pos);
void fn_710020a5a0(UnkStruct_710020a5a0* self, const base::math::CVector2D* scale);

struct UnkQuadRender_ovb0259 {
    u8 pad_0x0[0x78];
    void* field_0x78;
};

struct UnkQuadNode_ovb0259 {
    u8 pad_0x0[0x18];
    u8 node[0x160];  // 0x18
    UnkQuadRender_ovb0259* render;  // 0x178
};

struct UnkFadeQuads_ovb0259 {
    UnkQuadNode_ovb0259* second;  // 0x0
    UnkQuadNode_ovb0259* first;   // 0x8
    f32 field_0x10;
    f32 field_0x14;
};

void fn_710035d7bc(UnkFadeQuads_ovb0259* self);

void fn_710035d5c4(UnkFadeQuads_ovb0259* self, f32 t) {
    bool release = false;
    if (g_unk_71727bc558.field_0x28) {
        UnkCameraCtrl* camera = fn_71010e2080(fn_7100defcec(g_unk_7101d47310));
        release = camera && fn_710009d50c(camera->vf_0x0(), fn_71010e7f90());
        if (!camera)
            release = false;
    } else {
        release = true;
    }
    if (release) {
        if (self->first && self->second) {
            --g_unk_71726b21d4;
            fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(self->first), 0);
            self->first = nullptr;
            if (self->second) {
                fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(self->second), 0);
                self->second = nullptr;
            }
        }
        return;
    }
    fn_710035d7bc(self);
    if (t <= 0.0f)
        t = 0.0f;
    else if (t >= 1.0f)
        t = 1.0f;
    fn_710020ac34(reinterpret_cast<UnkToggle*>(self->second->node), true);
    fn_710020ac34(reinterpret_cast<UnkToggle*>(self->first->node), true);
    alignas(8) f32 pos[3];
    alignas(8) f32 scale[2];
    scale[1] = 0.01f;
    scale[0] = t * 0.4f;
    fn_710020a5a0(reinterpret_cast<UnkStruct_710020a5a0*>(self->first->node), reinterpret_cast<const base::math::CVector2D*>(scale));
    if (t > 0.66f)
        fn_710019c564(self->first->render->field_0x78, 0.0f, 1.0f, 0.0f, 1.0f);
    else if (t > 0.33f)
        fn_710019c564(self->first->render->field_0x78, 1.0f, 0.5f, 0.0f, 1.0f);
    else
        fn_710019c564(self->first->render->field_0x78, 1.0f, 0.0f, 0.0f, 1.0f);
    f32 x = self->field_0x10 + (1.0f - t) * -0.4f;
    u32 y;
    __builtin_memcpy(&y, &self->field_0x14, 4);
    UnkQuadNode_ovb0259* node = self->first;
    pos[0] = x;
    __builtin_memcpy(&pos[1], &y, 4);
    pos[2] = 0.0f;
    fn_710020a4c4(node->node, *reinterpret_cast<const UnkMinimapVec3*>(pos));
}
