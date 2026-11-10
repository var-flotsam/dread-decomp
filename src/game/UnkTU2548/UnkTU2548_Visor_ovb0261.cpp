// Unit 2548 (tools/tu_map.py, no reliable name): sets up the visor texture's scene node once (the
// texture "system/dev/visor_samus.bctex", on layer 2, hidden, additive-less, white) and stores the
// HUD's size and position values. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "engine/scene/UnkToggle.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkQuadNode_ovb0259;
struct UnkStruct_710020a5a0;
struct UnkMinimapVec3;
struct UnkBlendLayer_710019c284;

void fn_710035d9e0(void* pool, const CFilePathStrId* path, UnkQuadNode_ovb0259** out, const CStrId* name);
void fn_710020a5a0(UnkStruct_710020a5a0* self, const base::math::CVector2D* scale);
void fn_710020a4c4(void* p, const UnkMinimapVec3& pos);
void fn_710019c284(UnkBlendLayer_710019c284* self);
void fn_710019c564(void* target, f32 a, f32 b, f32 c, f32 d);
void fn_710020ad90(void* self, s32 value);

struct UnkVisorRender_b0261 {
    u8 pad_0x0[0x78];
    u8* field_0x78;
    u8 pad_0x80[8];
    u32 field_0x88;
};

struct UnkVisorWidget_b0261 {
    u8 pad_0x0[0x18];
    u8 field_0x18[0x160];
    UnkVisorRender_b0261* render;  // 0x178
};

struct UnkVisorZero_b0261 {
    u64 a;
    u32 b;
    UnkVisorZero_b0261() : a(0), b(0) {}
};

struct UnkVisorHud_b0261 {
    UnkVisorWidget_b0261* widget;  // 0x0
    u8 pad_0x8[0x14];
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    u16 field_0x2c;
};

void fn_7100ddbd18(UnkVisorHud_b0261* self) {
    if (!self->widget) {
        CFilePathStrId path(base::global::UnkStrIdInline(), "system/dev/visor_samus.bctex", true);
        UnkRenderManager* manager;
        {
            UnkQuadNode_ovb0259* created = nullptr;
            manager = g_unk_7101d47300;
            fn_710035d9e0(manager->field_0x218, &path, &created, &g_unk_7101d470e0);
            fn_7100219ae0(manager, reinterpret_cast<UnkSceneNode*>(created), 2);
            self->widget = reinterpret_cast<UnkVisorWidget_b0261*>(created);
        }
        fn_710020ac34(reinterpret_cast<UnkToggle*>(self->widget->field_0x18), false);
        u8* layer = self->widget->render->field_0x78;
        layer[0x181] = 0;
        fn_710019c284(reinterpret_cast<UnkBlendLayer_710019c284*>(layer));
        __builtin_memset(&self->widget->render->field_0x88, 0, 4);
        fn_710019c564(self->widget->render->field_0x78, 1.0f, 1.0f, 1.0f, 1.0f);
        fn_710020ad90(self->widget->field_0x18, -21);
        {
            void* node = self->widget->field_0x18;
            UnkVisorZero_b0261 zero;
            fn_710020a4c4(node, *reinterpret_cast<const UnkMinimapVec3*>(&zero));
        }
        {
            UnkVisorWidget_b0261* widget = self->widget;
            base::math::CVector2D scale;
            scale.x = 1.0f;
            scale.y = 0.5625f;
            fn_710020a5a0(reinterpret_cast<UnkStruct_710020a5a0*>(widget->field_0x18), &scale);
        }
    }
    self->field_0x1c = 600.0f;
    self->field_0x20 = 5.0f;
    self->field_0x24 = 0.5f;
    self->field_0x28 = 0.0f;
    self->field_0x2c = 0;
}
