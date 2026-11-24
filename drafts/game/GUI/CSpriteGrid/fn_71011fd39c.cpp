// GUI::CSpriteGrid unit: lays out the sprite's nine-slice grid. Each of the 3x3 cells shows a third of the
// source rectangle; the corner columns and rows take the scaled size of a third of the texture, the middle
// ones what is left of the object's size, and the sprite is centred on the object. Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkTU1722.h"
#include "types.h"

using base::math::CVector2D;

// A 2D position returned in memory (its user-provided copy keeps it out of registers).
struct UnkGuiPos {
    f32 x;
    f32 y;

    UnkGuiPos(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkGuiPos(const UnkGuiPos& other) : x(other.x), y(other.y) {}
};

struct UnkGuiAnchor;
UnkGuiPos fn_71011f6344(const UnkGuiAnchor* self);
UnkLabelVec2 fn_71011b7ec0(s32 key, const UnkLabelVec2* v, s32 mode);

struct UnkSpriteGridCells;
struct UnkSpriteTransform;

// The sprite's position and depth.
struct UnkSpritePoint_l1fd39c {
    f32 x;
    f32 y;
    f32 z;
};

void fn_7100195eb4(UnkSpriteGridCells* grid, u32 col, u32 row, const CVector2D* pos, const CVector2D* size);
void fn_7100195e5c(UnkSpriteGridCells* grid, u32 col, f32 width);
void fn_7100195e88(UnkSpriteGridCells* grid, u32 row, f32 height);
void fn_710020b148(UnkSpriteTransform* self, const UnkSpritePoint_l1fd39c* pos);

struct UnkSpriteMesh_l1fd39c {
    u8 pad_0x0[0x78];
    UnkSpriteGridCells* grid;
};

// The sprite the grid draws: its transform at +0x18, its mesh at +0x128 and a flag that it must be rebuilt.
struct UnkGridSprite_l1fd39c {
    u8 pad_0x0[0x18];
    u8 transform[0xf8];
    u8 pad_0x110[0x3];
    bool dirty;
    u8 pad_0x114[0x14];
    UnkSpriteMesh_l1fd39c* mesh;

    void SetPosition(const UnkSpritePoint_l1fd39c& pos) {
        fn_710020b148(reinterpret_cast<UnkSpriteTransform*>(transform), &pos);
    }
    void SetCell(u32 col, u32 row, const CVector2D* pos, const CVector2D* size) {
        fn_7100195eb4(mesh->grid, col, row, pos, size);
        dirty = true;
    }
    void SetColumnWidth(u32 col, f32 width) {
        fn_7100195e5c(mesh->grid, col, width);
        dirty = true;
    }
    void SetRowHeight(u32 row, f32 height) {
        fn_7100195e88(mesh->grid, row, height);
        dirty = true;
    }
};

// The texture's source rectangle.
struct UnkGridSource_l1fd39c {
    u8 pad_0x0[0x8];
    f32 x;
    f32 y;
    f32 width;
    f32 height;
};

// The object's derived state as the grid reads it: its size, its scale, its depth and its frame.
struct UnkGridState_l1fd39c {
    u8 pad_0x0[0x14];
    f32 width;
    f32 height;
    f32 scale_x;
    f32 scale_y;
    f32 depth;
    u8 pad_0x28[0x10];
    s32 frame;
};

struct UnkSpriteGrid_l1fd39c {
    u8 pad_0x0[0x190];
    UnkGridSprite_l1fd39c* sprite;
    u8 pad_0x198[0x8];
    UnkGridSource_l1fd39c* source;
    u8 pad_0x1a8[0x3a];
    bool laid_out;
};

void fn_71011fd39c(UnkSpriteGrid_l1fd39c* self) {
    UnkGridState_l1fd39c* state =
        reinterpret_cast<UnkGridState_l1fd39c*>(fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self)));
    if (self->sprite != nullptr) {
        const UnkGridSource_l1fd39c* source = self->source;
        f32 x = source->x;
        f32 y = source->y;
        f32 source_w = source->width;
        f32 source_h = source->height;
        UnkLabelVec2 texture = GUI_GetImageRegionSizeInFrame(reinterpret_cast<const UnkLabelSource*>(source), state->frame);
        CVector2D cell = {source_w * (1.0f / 3.0f), source_h * (1.0f / 3.0f)};

        f32 width = state->width * state->scale_x;
        f32 height = state->height * state->scale_y;
        UnkLabelVec2 margin((state->scale_x - 1.0f) * 6.0f, (state->scale_y - 1.0f) * 6.0f);
        UnkLabelVec2 border = fn_71011b8078(state->frame, margin);
        UnkLabelVec2 inner(width - (border.x + border.x), height - (border.y + border.y));
        UnkGuiPos origin = fn_71011f6344(reinterpret_cast<const UnkGuiAnchor*>(state));
        f32 half_x = (width - inner.x) * 0.5f;
        f32 half_y = (height - inner.y) * 0.5f;
        UnkLabelVec2 center(half_x + origin.x, half_y + origin.y);
        UnkLabelVec2 placed = fn_71011b7ec0(state->frame, &center, 0);
        self->sprite->SetPosition(UnkSpritePoint_l1fd39c{placed.x, placed.y, state->depth});

        UnkLabelVec2 corner(texture.x * (1.0f / 3.0f), texture.y * (1.0f / 3.0f));
        for (u32 row = 0; row < 3; ++row) {
            for (u32 col = 0; col < 3; ++col) {
                CVector2D pos = {x + cell.x * col, y + cell.y * row};
                self->sprite->SetCell(col, row, &pos, &cell);
                if (col != 1 && row != 1) {
                    if (col == 0)
                        inner.y -= corner.y;
                    if (row == 0)
                        inner.x -= corner.x;
                    UnkLabelVec2 edge = fn_71011b7f48(state->frame, corner);
                    edge.x += edge.x;
                    edge.y += edge.y;
                    self->sprite->SetColumnWidth(col, edge.x);
                    self->sprite->SetRowHeight(row, edge.y);
                }
            }
        }

        {
            UnkLabelVec2 middle = fn_71011b7f48(state->frame, inner);
            middle.x += middle.x;
            middle.y += middle.y;
            self->sprite->SetColumnWidth(1, middle.x);
            self->sprite->SetRowHeight(1, middle.y);
        }
    }
    self->laid_out = true;
}
