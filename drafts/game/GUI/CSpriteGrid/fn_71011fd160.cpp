// GUI::CSpriteGrid's unit: the size of one cell, the sprite sheet's texture size scaled by the model and
// then by the object's frame, or (-1, -1) when there is none. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

struct UnkSpriteGridTexture_ovb0043 {
    u8 pad_0x0[0x10];
    u32 width;  // 0x10
    u32 height;  // 0x14
};

struct UnkSpriteGridModel_ovb0043 {
    u8 pad_0x0[0x10];
    f32 scale_x;  // 0x10
    f32 scale_y;  // 0x14
    UnkSpriteGridTexture_ovb0043* texture;  // 0x18
};

struct UnkSpriteGrid_ovb0043 {
    void** vtable;
    u8 pad_0x8[0x188];
    void* field_0x190;
    u8 pad_0x198[0x8];
    UnkSpriteGridModel_ovb0043* model;  // 0x1a0
    u8 pad_0x1a8[0x2c];
    u32 columns;  // 0x1d4
    u32 rows;  // 0x1d8
};

// A size whose assignment copies the two components one by one.
struct UnkGridSize_ovb0043 {
    f32 x;
    f32 y;
    UnkGridSize_ovb0043(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkGridSize_ovb0043(const UnkGridSize_ovb0043& other) : x(other.x), y(other.y) {}
    UnkGridSize_ovb0043& operator=(const UnkGridSize_ovb0043& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
    UnkGridSize_ovb0043& operator=(const UnkLabelVec2& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

UnkGridSize_ovb0043 fn_71011cf214(UnkSpriteGrid_ovb0043* self, const UnkGridSize_ovb0043* size);

// The model's texture size (width and height), scaled by the model and then by the frame `frame`.
static inline void GetScaledTextureSize(UnkGridSize_ovb0043* out, const UnkSpriteGridModel_ovb0043* model,
                                        u32 width, u32 height, s32 frame) {
    base::math::CVector2D scale = {model->scale_x, model->scale_y};
    f32 h = (f32)height;
    f32 w = (f32)width;
    base::math::CVector2D size = {scale.x * w, scale.y * h};
    *out = fn_71011b7fc8(g_unk_717271e8f8, frame, &size);
}

UnkGridSize_ovb0043 fn_71011fd160(UnkSpriteGrid_ovb0043* self) {
    UnkGridSize_ovb0043 result(-1.0f, -1.0f);
    UnkGridSize_ovb0043 cell(-1.0f, -1.0f);
    UnkGuiObjectState* state = fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self));
    if (self->field_0x190 && state->frame != 0x7fffffff) {
        UnkSpriteGridModel_ovb0043* model = self->model;
        u32 width = model->texture->width;
        u32 height = model->texture->height;
        if (width != 0 && height != 0) {
            GetScaledTextureSize(&cell, model, width, height, state->frame);
            if (cell.x >= 0.0f && cell.y >= 0.0f)
                result = fn_71011cf214(self, &cell);
        }
    }
    return result;
}
