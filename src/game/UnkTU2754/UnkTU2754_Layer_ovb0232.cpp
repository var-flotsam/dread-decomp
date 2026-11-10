// Unit 2754 of tools/tu_map.py (no reflected class): a screen layer item that sizes itself to its
// render layer's dimensions (fn_71011f5cd0) and, when given a new name and layer, re-registers with
// the render manager (fn_71011f5b9c). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CStrId;

void fn_7100219804(UnkRenderManager* self, u32 layer);
void fn_71002198cc(UnkRenderManager* self, void* a, void* b, void* c, u32 layer, u32 flag);
void fn_7100219bdc(UnkRenderManager* self, u32* width, u32* height, u32 layer);
struct UnkSceneObj_71010e2278;
void fn_7100219bb4(UnkRenderManager* mgr, UnkSceneObj_71010e2278* obj, u32 layer);

struct UnkScreenLayer_b0232 {
    CStrId name;  // 0x0
    u32 layer;  // 0x8
    u8 pad_0xc[0xc];
    UnkSceneObj_71010e2278* object;  // 0x18
    u8 pad_0x20[4];
    f32 field_0x24, field_0x28, field_0x2c, field_0x30, field_0x34, field_0x38, field_0x3c, field_0x40;
};

void fn_71011f5d80(UnkScreenLayer_b0232* self);

// Reads the layer's width and height and derives the scale values the item keeps.
static inline void FitToLayer(UnkScreenLayer_b0232* self) {
    u32 width, height;
    fn_7100219bdc(g_unk_7101d47300, &width, &height, self->layer);
    f32 w = (f32)width;
    self->field_0x24 = w;
    self->field_0x28 = 1.0f / w;
    f32 h = (f32)height;
    f32 ratio = w / h;
    self->field_0x2c = h;
    self->field_0x30 = 1.0f / h;
    f32 inv = 1.0f / ratio;
    self->field_0x34 = ratio;
    self->field_0x38 = inv;
    f32 two = inv + inv;
    self->field_0x3c = two;
    self->field_0x40 = (2.0f - two) * 0.5f;
}

void fn_71011f5cd0(UnkScreenLayer_b0232* self) {
    fn_7100219804(g_unk_7101d47300, self->layer);
    fn_71002198cc(g_unk_7101d47300, nullptr, nullptr, nullptr, self->layer, 1);
    FitToLayer(self);
}

void fn_71011f5b9c(UnkScreenLayer_b0232* self, const CStrId& name, u32 layer) {
    self->name = name;
    self->layer = layer;
    fn_7100219804(g_unk_7101d47300, layer);
    fn_71002198cc(g_unk_7101d47300, nullptr, nullptr, nullptr, self->layer, 1);
    u32 width, height;
    fn_7100219bdc(g_unk_7101d47300, &width, &height, self->layer);
    f32 w = (f32)width;
    self->field_0x24 = w;
    self->field_0x28 = 1.0f / w;
    f32 h = (f32)height;
    f32 ratio = w / h;
    self->field_0x2c = h;
    self->field_0x30 = 1.0f / h;
    f32 inv = 1.0f / ratio;
    self->field_0x34 = ratio;
    self->field_0x38 = inv;
    f32 two = inv + inv;
    self->field_0x3c = two;
    self->field_0x40 = (2.0f - two) * 0.5f;
    fn_71011f5d80(self);
    if (self->object)
        fn_7100219bb4(g_unk_7101d47300, self->object, self->layer);
}
