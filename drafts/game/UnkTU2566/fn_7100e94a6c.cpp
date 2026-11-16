// Unit 2566 (CMinimapManager::CTunableMinimapManager): recomputes the minimap's bounds from its base
// rectangle, the area's and map's rectangles, the current area's points and the markers, and
// limits them around the focus while the view is held. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

typedef u32 __attribute__((may_alias)) a_u32;

// A rectangle returned in memory, copied word by word (as in src/game/CMinimapManager.cpp).
struct UnkMinimapRect {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    UnkMinimapRect() {}
    UnkMinimapRect(f32 x0, f32 y0, f32 x1, f32 y1) : min_x(x0), min_y(y0), max_x(x1), max_y(y1) {}
    UnkMinimapRect(const UnkMinimapRect& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
    }
    UnkMinimapRect& operator=(const UnkMinimapRect& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
        return *this;
    }
};

// Something placed on the map, at (x, y).
struct UnkMinimapPoint_94a6c {
    u8 pad_0x0[8];
    f32 x;
    f32 y;
};

// The points of an area.
struct UnkMinimapPoints_94a6c {
    UnkMinimapPoint_94a6c* data;
    u32 count;
    u8 pad_0xc[0x1c];
};
static_assert(sizeof(UnkMinimapPoints_94a6c) == 0x28, "UnkMinimapPoints_94a6c layout");

// Areas' points by the area's id.
struct UnkMinimapPointMap_94a6c {
    UnkMinimapPoints_94a6c* values;
    CStrId* keys;
    u32 count;

    UnkMinimapPoints_94a6c* Find(const CStrId& key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return &values[count];
    }
    UnkMinimapPoints_94a6c* End() { return &values[count]; }
};

struct UnkMinimapMap_94a6c {
    u8 pad_0x0[0x6348];
    UnkMinimapRect bounds;
};

struct UnkMinimapOwner_94a6c {
    u8 pad_0x0[0x68];
    UnkMinimapMap_94a6c* map;
    u8 pad_0x70[0xa0];
    CStrId area;  // 0x110
    u8 pad_0x118[0xd63bd0];
    UnkMinimapRect area_bounds;  // 0xd63ce8
    UnkMinimapRect base_bounds;  // 0xd63cf8
    u8 pad_0xd63d08[0x28];
    UnkMinimapRect view;    // 0xd63d30
    UnkMinimapRect bounds;  // 0xd63d40
    u8 pad_0xd63d50[0x18];
    bool use_map;   // 0xd63d68
    u8 pad_0xd63d69[2];
    bool use_area;  // 0xd63d6b
    CVector3D focus;
    u8 pad_0xd63d78[0xc8];
    u8 hold_timer[0x386];  // 0xd63e40
    bool limit;            // 0xd641c6
    u8 pad_0xd641c7[0x19];
    UnkMinimapPoint_94a6c** markers;  // 0xd641e0
    u32 marker_count;
    u8 pad_0xd641ec[0xb4];
    UnkMinimapPointMap_94a6c area_points;  // 0xd642a0
};
static_assert(__builtin_offsetof(UnkMinimapOwner_94a6c, area_bounds) == 0xd63ce8, "UnkMinimapOwner_94a6c layout");
static_assert(__builtin_offsetof(UnkMinimapOwner_94a6c, limit) == 0xd641c6, "UnkMinimapOwner_94a6c layout");
static_assert(__builtin_offsetof(UnkMinimapOwner_94a6c, area_points) == 0xd642a0, "UnkMinimapOwner_94a6c layout");

// The lesser and greater of two values (as std::min and std::max order their compares).
static inline f32 Min(f32 a, f32 b) { return b < a ? b : a; }
static inline f32 Max(f32 a, f32 b) { return a < b ? b : a; }

// Grows a rectangle to take in a point.
static inline void TakeIn(UnkMinimapRect& b, const UnkMinimapPoint_94a6c& p) {
    f32 minX = Min(p.x, b.min_x);
    f32 minY = Min(p.y, b.min_y);
    b.min_x = minX;
    b.min_y = minY;
    f32 maxX = Max(p.x, b.max_x);
    f32 maxY = Max(p.y, b.max_y);
    b.max_x = maxX;
    b.max_y = maxY;
}

void fn_7100e94a6c(UnkMinimapOwner_94a6c* self) {
    UnkMinimapRect& b = self->bounds;
    b = self->base_bounds;
    if (self->use_area) {
        const UnkMinimapRect& a = self->area_bounds;
        b.min_x = Min(a.min_x, b.min_x);
        b.min_y = Min(a.min_y, b.min_y);
        b.max_x = Max(a.max_x, b.max_x);
        b.max_y = Max(a.max_y, b.max_y);
    }
    if (self->use_map) {
        UnkMinimapRect m = self->map->bounds;
        b.min_x = Min(m.min_x, b.min_x);
        b.min_y = Min(m.min_y, b.min_y);
        b.max_x = Max(m.max_x, b.max_x);
        b.max_y = Max(m.max_y, b.max_y);
    }
    UnkMinimapPointMap_94a6c& map = self->area_points;
    if (map.Find(self->area) != map.End()) {
        UnkMinimapPoints_94a6c* points = map.Find(self->area);
        for (u32 i = 0; i < points->count; ++i)
            TakeIn(b, points->data[i]);
    }
    for (u32 i = 0; i < self->marker_count; ++i)
        TakeIn(b, *self->markers[i]);
    if (!(b.max_x >= b.min_x && b.max_y >= b.min_y))
        b = UnkMinimapRect(0.0f, 0.0f, 0.0f, 0.0f);
    if (self->limit && !fn_710002fb70(self->hold_timer)) {
        CVector2D low;
        CVector2D high;
        fn_710007b430(&low, &self->focus);
        f32 minX = low.x - (self->view.max_x - self->view.min_x) * 0.5f * 0.8f + 900.0f;
        f32 minY = low.y - (self->view.max_y - self->view.min_y) * 0.5f * 0.8f + 900.0f;
        fn_710007b430(&high, &self->focus);
        f32 maxX = high.x + (self->view.max_x - self->view.min_x) * 0.5f * 0.8f - 900.0f;
        f32 maxY = high.y + (self->view.max_y - self->view.min_y) * 0.5f * 0.8f - 900.0f;
        b.min_x = Max(minX, b.min_x);
        b.min_y = Max(minY, b.min_y);
        b.max_x = Min(maxX, b.max_x);
        b.max_y = Min(maxY, b.max_y);
    }
}