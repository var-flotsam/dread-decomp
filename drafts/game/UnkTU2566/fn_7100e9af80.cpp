// Unit 2566 (tools/tu_map.py; hint CMinimapManager::CTunableMinimapManager): finds the map's segment for a
// usable's id and focuses the map view on a rectangle around it, wider across a vertical segment and
// taller above a horizontal one. Names and layouts are placeholders.
#include "types.h"

// As src/game/UnkTU2566/UnkTU2566_Minimap_ovb0200.cpp declares them.
struct UnkUsableInfo_b0200 {
    u8 pad_0x0[0x30];
};
struct UnkSelf_b0200;

// As src/game/UnkTU1689/UnkTU1689_MapMenu_ovb0203.cpp declares them.
// A rectangle given as float bits (the compiler keeps these four stores apart that way).
struct UnkRect_ovb0203 {
    u32 x0, y0, x1, y1;
};
void fn_7100e79654(void* target, const UnkRect_ovb0203* rect, u32 mode, u32 flags, void* extra);

struct UnkVec2_le9af80 {
    f32 x;
    f32 y;

    UnkVec2_le9af80 operator+(const UnkVec2_le9af80& o) const { return UnkVec2_le9af80{x + o.x, y + o.y}; }
};

// The smallest rectangle around the points added to it; it starts inside out.
struct UnkBounds_le9af80 {
    UnkVec2_le9af80 min;
    UnkVec2_le9af80 max;

    UnkBounds_le9af80() {
        min.x = 3.4028235e38f;
        min.y = 3.4028235e38f;
        max.x = -3.4028235e38f;
        max.y = -3.4028235e38f;
    }

    void Add(const UnkVec2_le9af80& p) {
        min.x = min.x < p.x ? min.x : p.x;
        min.y = min.y < p.y ? min.y : p.y;
        max.x = max.x > p.x ? max.x : p.x;
        max.y = max.y > p.y ? max.y : p.y;
    }
};

// A segment of the map (0x80 bytes): its two ends, as a rectangle.
struct UnkMapSegment_le9af80 {
    UnkBounds_le9af80 ends;
    u8 pad_0x10[0x70];
};

struct UnkMapSegments_le9af80 {
    u8 pad_0x0[0x62a8];
    UnkMapSegment_le9af80* values;  // 0x62a8
    u64* keys;                      // 0x62b0
    u32 count;                      // 0x62b8

    UnkMapSegment_le9af80* Find(const u64& key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return &values[count];
    }
};

struct UnkMapView_le9af80 {
    u8 pad_0x0[0x68];
    UnkMapSegments_le9af80* segments;  // 0x68
    u8 pad_0x70[0xa8];
    u8 view[1];  // 0x118
};

struct UnkUsableKey_le9af80 {
    u64 id;
};

// Adds both ends of the segment moved by `offset`.
static inline void AddSegment(UnkBounds_le9af80& bounds, const UnkMapSegment_le9af80* segment,
                              const UnkVec2_le9af80& offset) {
    bounds.Add(segment->ends.min + offset);
    bounds.Add(UnkVec2_le9af80{segment->ends.max.x + offset.x, segment->ends.max.y + offset.y});
}

void fn_7100e9af80(UnkSelf_b0200* self_, UnkUsableInfo_b0200* info) {
    UnkMapView_le9af80* self = reinterpret_cast<UnkMapView_le9af80*>(self_);
    UnkMapSegments_le9af80* segments = self->segments;
    UnkMapSegment_le9af80* segment = segments->Find(reinterpret_cast<UnkUsableKey_le9af80*>(info)->id);
    if (segment == &segments->values[segments->count])
        return;
    UnkBounds_le9af80 bounds;
    if (segment->ends.min.y != segment->ends.max.y) {
        AddSegment(bounds, segment, UnkVec2_le9af80{-300.0f, 0.0f});
        AddSegment(bounds, segment, UnkVec2_le9af80{300.0f, 0.0f});
    } else {
        AddSegment(bounds, segment, UnkVec2_le9af80{0.0f, -200.0f});
        AddSegment(bounds, segment, UnkVec2_le9af80{0.0f, 700.0f});
    }
    fn_7100e79654(self->view, reinterpret_cast<const UnkRect_ovb0203*>(&bounds), 4, 0, nullptr);
}
