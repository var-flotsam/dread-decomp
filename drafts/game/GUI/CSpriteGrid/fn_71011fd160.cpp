// GUI::CSpriteGrid's unit: the size of one cell, the sprite sheet's texture size scaled by the model and
// then by the object's frame, or (-1, -1) when there is none. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

struct UnkCellTexture_l1fd160 {
    u8 pad_0x0[0x10];
    u32 width;   // 0x10
    u32 height;  // 0x14
};

struct UnkCellModel_l1fd160 {
    u8 pad_0x0[0x10];
    f32 scale_x;  // 0x10
    f32 scale_y;  // 0x14
    UnkCellTexture_l1fd160* texture;  // 0x18
};

struct UnkCellGrid_l1fd160 {
    void** vtable;
    u8 pad_0x8[0x188];
    void* field_0x190;
    u8 pad_0x198[0x8];
    UnkCellModel_l1fd160* model;  // 0x1a0
};

struct UnkCellSize_l1fd160 {
    f32 x;
    f32 y;
    UnkCellSize_l1fd160() {}
    UnkCellSize_l1fd160(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkCellSize_l1fd160(const UnkCellSize_l1fd160& other) : x(other.x), y(other.y) {}
    UnkCellSize_l1fd160& operator=(const UnkCellSize_l1fd160& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
    UnkCellSize_l1fd160& operator=(const UnkLabelVec2& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

UnkCellSize_l1fd160 fn_71011cf214(UnkCellGrid_l1fd160* self, const UnkCellSize_l1fd160* size);

UnkCellSize_l1fd160 fn_71011fd160(UnkCellGrid_l1fd160* self) {
    UnkCellSize_l1fd160 result(-1.0f, -1.0f);
    UnkCellSize_l1fd160 scaled;
    UnkCellSize_l1fd160 frame;
    UnkCellSize_l1fd160 unit(-1.0f, -1.0f);
    UnkGuiObjectState* state = fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self));
    if (self->field_0x190 && state->frame != 0x7fffffff) {
        UnkCellModel_l1fd160* model = self->model;
        u32 w = model->texture->width;
        u32 h = model->texture->height;
        if (w != 0 && h != 0) {
            f32 sy = (f32)h;
            f32 sx = (f32)w;
            scaled = UnkCellSize_l1fd160(model->scale_x * sx, model->scale_y * sy);
            frame = fn_71011b7fc8(g_unk_717271e8f8, state->frame, reinterpret_cast<base::math::CVector2D*>(&scaled));
            unit = frame;
            if (unit.x >= 0.0f && unit.y >= 0.0f) {
                result = fn_71011cf214(self, &unit);
            }
        }
    }
    return result;
}
