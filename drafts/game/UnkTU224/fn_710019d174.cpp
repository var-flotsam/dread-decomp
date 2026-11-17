// Unit 224 (tools/tu_map.py): rebuilds a grid of quads, one per cell, from each cell's size, offset and
// advance, laying the cells out row by row in clip space; optionally also their texture coordinates and
// colour. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkTU53.h"
#include "types.h"

using base::global::CRntVector;
using base::math::CVector2D;

// As src/game/UnkFrameSource.cpp declares them.
template <class T>
struct UnkFrameVector;
struct UnkFramePair;

UnkFramePair* fn_710019cf9c(UnkFrameVector<UnkFramePair>* v, u32 i);
f32* fn_71000d4a50(UnkFrameVector<f32>* v, u32 i);

struct UnkQuadGrid_l19d174 {
    u8 pad_0x0[0x20];
    u32 columns;  // 0x20
    u32 rows;     // 0x24
    u8 pad_0x28[0x24];
    f32 line_height;  // 0x4c
    u8 pad_0x50[0x8];
    f32 top;  // 0x58
    f32 u;    // 0x5c
    f32 v;    // 0x60
    f32 du;   // 0x64
    f32 dv;   // 0x68
    f32 color[4];  // 0x6c
    u8 pad_0x7c[0x4];
    void* field_0x80;
    f32* positions;  // 0x88, two per vertex
    f32* uvs;        // 0x90, two per vertex
    f32* colors;     // 0x98, four per vertex
    CRntVector<CVector2D> sizes;    // 0xa0
    CRntVector<CVector2D> offsets;  // 0xc8
    CRntVector<f32> advances;       // 0xf0
    u8 pad_0x118[0x95];
    bool dirty;        // 0x1ad
    bool field_0x1ae;  // set once the quads are built

    const CVector2D* Size(u32 cell) {
        return reinterpret_cast<CVector2D*>(
            fn_710019cf9c(reinterpret_cast<UnkFrameVector<UnkFramePair>*>(&sizes), cell));
    }
    const CVector2D* Offset(u32 cell) {
        return reinterpret_cast<CVector2D*>(
            fn_710019cf9c(reinterpret_cast<UnkFrameVector<UnkFramePair>*>(&offsets), cell));
    }
    f32 Advance(u32 cell) { return *fn_71000d4a50(reinterpret_cast<UnkFrameVector<f32>*>(&advances), cell); }

    // The vertex at `corner` (0 to 3: top left, top right, bottom left, bottom right) of a cell.
    u32 Vertex(u32 column, u32 row, u32 corner) { return (rows * column + row) * 4 + corner; }

    void SetPosition(u32 vertex, f32 x, f32 y) {
        f32* p = &positions[vertex * 2];
        p[0] = x;
        p[1] = y;
        p[0] = fn_71000768c0(p[0]);
        p[1] = fn_71000768f0(p[1]);
    }

    // A vertex's texture coordinates (the right or bottom edge of the texture rectangle when `right`
    // or `bottom`) and its colour.
    void SetAttributes(u32 vertex, bool right, bool bottom) {
        f32* uv = &uvs[vertex * 2];
        f32* c = &colors[vertex * 4];
        uv[0] = right ? u + du : u;
        uv[1] = bottom ? v + dv : v;
        c[0] = color[0];
        c[1] = color[1];
        c[2] = color[2];
        c[3] = color[3];
    }
};
static_assert(__builtin_offsetof(UnkQuadGrid_l19d174, sizes) == 0xa0, "UnkQuadGrid_l19d174 layout");
static_assert(__builtin_offsetof(UnkQuadGrid_l19d174, dirty) == 0x1ad, "UnkQuadGrid_l19d174 layout");

void fn_710019d174(UnkQuadGrid_l19d174* self, bool with_attributes) {
    if (!self->field_0x80 || !self->dirty)
        return;
    f32 y = fn_71000768c0(self->top + self->top);
    for (u32 row = 0; row < self->rows; ++row) {
        f32 x = fn_71000768f0(-0.0f);
        for (u32 column = 0; column < self->columns; ++column) {
            u32 cell = self->rows * column + row;
            const CVector2D* size = self->Size(cell);
            const CVector2D* offset = self->Offset(cell);
            f32 advance = self->Advance(cell);
            f32 left = x + offset->x;
            f32 top = y - (self->line_height - offset->y);
            f32 bottom = top - size->y;
            f32 right = left + size->x;
            if (with_attributes) {
                self->SetAttributes(self->Vertex(column, row, 0), false, false);
                self->SetPosition(self->Vertex(column, row, 0), left, top);
                self->SetAttributes(self->Vertex(column, row, 1), true, false);
                self->SetPosition(self->Vertex(column, row, 1), right, top);
                self->SetAttributes(self->Vertex(column, row, 2), false, true);
                self->SetPosition(self->Vertex(column, row, 2), left, bottom);
                self->SetAttributes(self->Vertex(column, row, 3), true, true);
                self->SetPosition(self->Vertex(column, row, 3), right, bottom);
            } else {
                self->SetPosition(self->Vertex(column, row, 0), left, top);
                self->SetPosition(self->Vertex(column, row, 1), right, top);
                self->SetPosition(self->Vertex(column, row, 2), left, bottom);
                self->SetPosition(self->Vertex(column, row, 3), right, bottom);
            }
            x += advance;
        }
        y -= self->line_height;
    }
    self->dirty = false;
    self->field_0x1ae = true;
}
