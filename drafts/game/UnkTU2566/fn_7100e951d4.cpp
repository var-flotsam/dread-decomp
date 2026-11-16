// Unit 2566 (CMinimapManager::CTunableMinimapManager): repaints the minimap texture's pixels of the
// listed grid cells, coloured by kind or by how recently they were visited, and hands the texture
// and the changed rectangle to the render passes. Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2566.h"
#include "types.h"

// The frame timer's current time.
struct UnkFrameClock_951d4 {
    u8 unk_0x0[0x2c];
    f32 time;
};

// A grid cell: its place, kind, when it was visited and revealed, and its neighbour flags.
struct UnkMinimapCell_951d4 {
    f32 x;
    f32 y;
    u8 kind;
    u8 unk_0x9[3];
    f32 visited_at;
    f32 revealed_at;
    u8 unk_0x14[0x2b];
    u8 links;  // 0x3f
    u8 field_0x40;
    bool dirty;
    bool field_0x42;
};
static_assert(__builtin_offsetof(UnkMinimapCell_951d4, links) == 0x3f, "UnkMinimapCell_951d4 layout");

struct UnkMinimapOrigin_951d4 {
    u8 unk_0x0[0x68];
    f32 x;
    f32 y;
};

struct UnkMinimapOwner_951d4 {
    u8 unk_0x0[0x68];
    UnkMinimapOrigin_951d4* map;
    u8 unk_0x70[0xa8];
    u8 grid[1];  // 0x118
};

struct UnkGridCell_e;
UnkGridCell_e* fn_7100e796a8(void* grid, u32 index);
struct UnkLinkedCell;
void fn_7100e78180(UnkLinkedCell* self);
struct UnkLinkedCell_e;
void fn_7100e78060(UnkLinkedCell_e* self);

extern u32 g_unk_7101aa9e38[][1024];

// A colour channel from 0 to 1 as a byte.
static inline f32 Channel(f32 v) {
    f32 c = v * 255.0f;
    return c > 0.0f ? (!(c >= 255.0f) ? c : 255.0f) : 0.0f;
}

void fn_7100e951d4(UnkMinimapOwner* owner, u32 count, void* items, u32 field_0x3) {
    if (count == 0)
        return;
    UnkMinimapOwner_951d4* self = reinterpret_cast<UnkMinimapOwner_951d4*>(owner);
    const u32* indices = static_cast<const u32*>(items);
    bool byKind = field_0x3 & 1;
    u32 maxX = 0;
    u32 minX = 0xffffffff;
    u32 minY = 0xffffffff;
    u32 maxY = 0;
    f32 now = reinterpret_cast<const UnkFrameClock_951d4*>(g_unk_7101d47328)->time;
    f32 cellSize = g_unk_710164be30 + g_unk_710164be30;
    for (u32 i = 0; i < count; ++i) {
        UnkMinimapCell_951d4* cell =
            reinterpret_cast<UnkMinimapCell_951d4*>(fn_7100e796a8(self->grid, indices[i]));
        if (!cell)
            continue;
        f32 red;
        f32 green;
        if (!byKind) {
            f32 visited = now - cell->visited_at;
            red = visited > 0.0f ? (!(visited >= 1.0f) ? visited : 1.0f) : 0.0f;
            f32 revealed = 0.0f;
            if (cell->kind == 6) {
                f32 t = now - cell->revealed_at;
                revealed = t <= 0.0f ? 0.0f : (!(t >= 1.0f) ? t : 1.0f);
            }
            green = revealed + red * 0.4f * (1.0f - revealed);
        } else {
            red = cell->kind > 1;
            green = cell->kind == 6;
        }
        f32 r = Channel(red);
        f32 g = Channel(green);
        if (cell->dirty)
            fn_7100e78180(reinterpret_cast<UnkLinkedCell*>(cell));
        u32 color = (u8)r;
        u8 links = cell->links;
        color |= (u32)(u8)g << 8;
        if (cell->field_0x42)
            fn_7100e78060(reinterpret_cast<UnkLinkedCell_e*>(cell));
        color |= (u32)links << 16;
        u32 x = (u32)((cell->x - self->map->x) / cellSize);
        u32 y = (u32)((cell->y - self->map->y) / cellSize);
        if (x >= 0x3ff)
            x = 0x3ff;
        if (y >= 0x1ff)
            y = 0x1ff;
        color |= (u32)cell->field_0x40 << 24;
        u32* pixel = &g_unk_7101aa9e38[y][x];
        if (*pixel == color)
            continue;
        *pixel = color;
        minX = x < minX ? x : minX;
        minY = y < minY ? y : minY;
        maxX = x > maxX ? x : maxX;
        maxY = y > maxY ? y : maxY;
    }
    if (minY > maxY || minX > maxX)
        return;
    fn_7100219294(g_unk_7101d47300, 0x6fc641f145352069, reinterpret_cast<const f32*>(g_unk_7101aa9e38), 6);
    u32 rect[4] = {minX, minY, maxX, maxY};
    fn_7100219294(g_unk_7101d47300, 0x7bc15ce967f525d2, reinterpret_cast<const f32*>(rect), 6);
}
