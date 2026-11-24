// Unit 2565: sets the grid's origin and extent, then marks in its bitmap every 100-unit cell (650 x 300)
// whose centre and four corners all lie off the navigation mesh. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

using base::math::CVector2D;

struct UnkCellOffset_le78338 {
    f32 x;
    f32 y;
    UnkCellOffset_le78338(f32 x, f32 y) : x(x), y(y) {}
};

struct UnkOffMeshGrid_le78338 {
    u8 pad_0x0[0x8];
    CVector2D origin;  // 0x8
    CVector2D extent;  // 0x10
    u8 pad_0x18[0x8];
    u32 words[6094];  // 0x20, one bit per cell
    u32 count;        // 0x5f58, cells set
    u8 pad_0x5f5c[0x4];
    u32 size;  // 0x5f60, cells in the bitmap

    void SetCell(u32 index, bool value) {
        if (index < size) {
            u32& word = words[index >> 5];
            u32 mask = 1u << (index & 31);
            if (value) {
                if (!(word & mask)) {
                    word |= mask;
                    count++;
                }
            } else {
                if (word & mask) {
                    word &= ~mask;
                    count--;
                }
            }
        }
    }
};
static_assert(__builtin_offsetof(UnkOffMeshGrid_le78338, count) == 0x5f58, "");
static_assert(__builtin_offsetof(UnkOffMeshGrid_le78338, size) == 0x5f60, "");

// Whether the point lies on the navigation mesh (the query finds a polygon).
inline bool IsOnMesh(UnkNavMesh* navmesh, const CVector2D& point) {
    return CNavMesh_FindCellIndexAtPoint(navmesh, &point, 1) != 0xffff;
}

// Whether the point moved by the offset lies on the navigation mesh.
inline bool IsOnMesh(UnkNavMesh* navmesh, const CVector2D& point, const UnkCellOffset_le78338& offset) {
    CVector2D moved;
    moved.x = point.x + offset.x;
    moved.y = point.y + offset.y;
    return IsOnMesh(navmesh, moved);
}

void fn_7100e78338(UnkOffMeshGrid_le78338* self, UnkNavMesh* navmesh, const CVector2D* origin,
                   const CVector2D* extent) {
    static const UnkCellOffset_le78338 kCorners[4] = {
        UnkCellOffset_le78338(45.0f, 45.0f),
        UnkCellOffset_le78338(45.0f, -45.0f),
        UnkCellOffset_le78338(-45.0f, 45.0f),
        UnkCellOffset_le78338(-45.0f, -45.0f),
    };
    self->origin.x = origin->x;
    self->origin.y = origin->y;
    self->extent.x = extent->x;
    self->extent.y = extent->y;
    for (u32 row = 0; row < 300; row++) {
        f32 y = (f32)row * 100.0f;
        for (u32 col = 0; col < 650; col++) {
            CVector2D center;
            center.x = (f32)col * 100.0f + self->origin.x + 50.0f;
            center.y = y + self->origin.y + 50.0f;
            bool on_mesh = IsOnMesh(navmesh, center);
            for (const UnkCellOffset_le78338& offset : kCorners) {
                if (on_mesh)
                    break;
                on_mesh = IsOnMesh(navmesh, center, offset);
            }
            self->SetCell(row * 650 + col, !on_mesh);
        }
    }
}
