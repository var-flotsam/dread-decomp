// CBreakableTileGroup's unit (tools/tu_map.py unit 2213): the tiles of the group a circle overlaps. With
// `contacts` set, each contact point found becomes a hit (the tile, the group's collider, the point, the
// direction from the circle's centre and the side of the tile it is on); otherwise each tile hit once.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

#include <math.h>

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkHit;
struct UnkCircle2D;
template <u32 N>
struct UnkContactBuffer;
template <u32 Offset>
struct UnkWeakRefAt;

bool fn_71001276b0(const UnkCircle2D* a, const void* b, UnkContactBuffer<8>* out, bool flag);
void fn_710092ef94(UnkWeakRefAt<0>* self, UnkWeakTarget* other);

// A circle: its centre and radius.
struct UnkTileCircle_lca257c {
    f32 x, y;
    f32 radius;
};

// An axis-aligned box: its lower and upper corners.
struct UnkTileBox_lca257c {
    CVector2D min;
    CVector2D max;
};

// One contact point: the position, a vector for each shape of the pair, the time of impact and flags.
struct UnkTileContact_lca257c {
    CVector3D pos;
    CVector3D normal;
    CVector3D side;
    f32 time;
    u32 flags;
};
static_assert(sizeof(UnkTileContact_lca257c) == 0x2c, "contact layout");

// Up to eight contact points after their count; reading past the count clears the points up to it.
struct UnkTileContacts_lca257c {
    u32 count;
    UnkTileContact_lca257c points[8];

    UnkTileContacts_lca257c() : count(0) {}
    UnkTileContact_lca257c& operator[](u32 i) {
        if (i >= count) {
            __builtin_memset(&points[count], 0, (i - count + 1) * sizeof(UnkTileContact_lca257c));
            count = i + 1;
        }
        return points[i];
    }
};

struct UnkTile_lca257c {
    u8 pad_0x0[0x28];
    CVector2D pos;  // 0x28
    u8 pad_0x30[0x8];
    s32 state;  // 0x38: 2 when broken
    u8 pad_0x3c[0x6c];
};
static_assert(sizeof(UnkTile_lca257c) == 0xa8, "tile layout");

struct UnkTileGroup_lca257c {
    u8 pad_0x0[0x18];
    f32 x;  // 0x18
    f32 y;  // 0x1c
    f32 z;  // 0x20
    u32 width;  // 0x24
    u32 height;  // 0x28
    u8 pad_0x2c[0x14];
    u32 tile_count;  // 0x40
    UnkTile_lca257c* tiles;  // 0x48
    u8 pad_0x50[0x7c];
    u32 field_0xcc;
    UnkWeakTarget* collider;  // 0xd0
};
static_assert(__builtin_offsetof(UnkTileGroup_lca257c, collider) == 0xd0, "group layout");

// A tile hit (0x50 bytes): the tile, the collider, the tile's index and the contact point.
struct UnkTileHit_lca257c {
    UnkTile_lca257c* tile;
    u8 collider[0x10];  // 0x8: a weak reference
    u32 index;  // 0x18
    UnkTileContact_lca257c contact;  // 0x1c
    u32 field_0x48;
};
static_assert(sizeof(UnkTileHit_lca257c) == 0x50, "hit layout");

static const f32 kTileSize = 100.0f;

// The cell of a coordinate along one axis, clamped to the grid.
static inline f32 Cell(f32 v, f32 origin, u32 cells) {
    f32 last = cells - 1;
    f32 cell = floorf((v - origin + 0.001f) / kTileSize);
    f32 clamped = cell >= last ? last : cell;
    return !(cell <= 0.0f) ? clamped : 0.0f;
}

static inline f32 Length(const CVector3D& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

bool fn_7100ca257c(void* where, void* filter, u32 flags, u32 max, UnkHit* hits, u32* count, f32 f) {
    UnkTileGroup_lca257c* self = static_cast<UnkTileGroup_lca257c*>(where);
    const CVector2D* center = static_cast<const CVector2D*>(filter);
    UnkTileHit_lca257c* out = reinterpret_cast<UnkTileHit_lca257c*>(hits);
    bool contacts = flags & 1;
    f32 radius = f;

    f32 column = Cell(center->x, self->x, self->width);
    f32 row = Cell(center->y, self->y, self->height);
    f32 reach = radius / kTileSize;
    reach = reach > 1.0f ? reach : 1.0f;
    if (column < -reach || row < -reach)
        return false;
    if (column >= reach + self->width || row >= reach + self->height)
        return false;

    UnkTileCircle_lca257c circle;
    circle.x = center->x;
    circle.y = center->y;
    circle.radius = radius;
    UnkTileContacts_lca257c found;
    bool hit = false;
    for (u32 i = 0; i < self->tile_count; ++i) {
        UnkTile_lca257c* tile = &self->tiles[i];
        if (tile->state == 2)
            continue;
        UnkTileBox_lca257c box;
        box.min = tile->pos;
        box.max.x = box.min.x + kTileSize;
        box.max.y = box.min.y + kTileSize;
        found.count = 0;
        if (!fn_71001276b0(reinterpret_cast<const UnkCircle2D*>(&circle), &box,
                           reinterpret_cast<UnkContactBuffer<8>*>(&found), true))
            continue;
        if (!contacts) {
            if (*count < max) {
                out[*count].tile = tile;
                ++*count;
                hit = true;
            }
            continue;
        }
        u32 n = found.count;
        f32 mid_x = (box.max.x + box.min.x) * 0.5f;
        f32 mid_y = (box.max.y + box.min.y) * 0.5f;
        for (u32 j = 0; j < n && *count < max; ++j) {
            UnkTileContact_lca257c& contact = found[j];
            UnkTileHit_lca257c& entry = out[*count];
            entry.tile = tile;
            entry.field_0x48 = self->field_0xcc;
            entry.contact.flags |= 0x40;
            fn_710092ef94(reinterpret_cast<UnkWeakRefAt<0>*>(entry.collider), self->collider);
            contact.pos.z = self->z;
            CVector2D at;
            fn_710007b430(&at, &contact.pos);

            // The side of the tile the point is on: the axis along which it is furthest from the middle.
            f32 dx = at.x - mid_x;
            f32 dy = at.y - mid_y;
            f32 sx = dx < 0.0f ? -1.0f : 1.0f;
            f32 sy = dy < 0.0f ? -1.0f : 1.0f;
            bool along_x = fabsf(dx) > fabsf(dy);
            f32 side_x = along_x ? sx : 0.0f;
            f32 side_y = along_x ? 0.0f : sy;

            CVector3D dir;
            dir.x = contact.pos.x - center->x;
            dir.y = contact.pos.y - center->y;
            dir.z = 0.0f;
            f32 scale = 1.0f / (Length(dir) + 0.0001f);
            contact.flags |= 0x30;
            contact.side.x = side_x;
            contact.side.y = side_y;
            contact.side.z = 0.0f;
            contact.normal.y = dir.y * scale;
            contact.normal.x = dir.x * scale;
            contact.normal.z = scale * dir.z;

            UnkTileHit_lca257c& filled = out[*count];
            filled.index = i;
            filled.contact = contact;
            filled.contact.flags |= 1;
            ++*count;
            hit = true;
        }
    }
    return hit;
}
