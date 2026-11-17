// CAreaFXComponent unit (tools/tu_map.py unit 1263): lays effect points along a path: for each segment, its
// start and then a point every spacing along it, each with the segment's angle (into a second list).
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

extern "C" float atan2f(float y, float x);

struct UnkPathVec_l802df0 {
    f32 x, y, z;

    void Set(f32 x_, f32 y_, f32 z_) {
        x = x_;
        y = y_;
        z = z_;
    }
};

// The positions: at most 400.
struct UnkPathPositions_l802df0 {
    u32 count;
    UnkPathVec_l802df0 data[400];

    bool IsFull() const { return count >= 400; }
    UnkPathVec_l802df0& Add() {
        data[count] = UnkPathVec_l802df0();
        return data[count++];
    }
};

// The rotations, one per position.
struct UnkPathRotations_l802df0 {
    u32 count;
    UnkPathVec_l802df0 data[1];

    UnkPathVec_l802df0& Add() {
        s64 index = -1;
        if (count != 0xffffffff) {
            data[count] = UnkPathVec_l802df0();
            index = count++;
        }
        return data[index];
    }
};

struct UnkPathPoint_l802df0 {
    CVector2D xy;
    f32 z;
};

struct UnkPathPoints_l802df0 {
    UnkPathPoint_l802df0* data;
};

struct UnkPathConfig_l802df0 {
    u8 pad_0x0[0x80];
    f32 spacing;  // 0x80
};

struct UnkPathSelf_l802df0 {
    u8 pad_0x0[0x10];
    UnkPathConfig_l802df0* config;  // 0x10
};

static inline f32 DistanceSq_l802df0(const CVector2D& a, const CVector2D& b) {
    f32 x = a.x - b.x;
    f32 y = a.y - b.y;
    return x * x + y * y;
}

static inline void AddPoint_l802df0(UnkPathPositions_l802df0* positions, UnkPathRotations_l802df0* rotations,
                                    const CVector2D& pos, f32 z, f32 angle) {
    if (positions->IsFull())
        return;
    positions->Add().Set(pos.x, pos.y, z);
    rotations->Add().Set(0.0f, 0.0f, angle);
}

void fn_7100802df0(UnkPathSelf_l802df0* self, UnkPathPoints_l802df0* points, UnkPathPositions_l802df0* positions,
                   UnkPathRotations_l802df0* rotations, u32 count, f32 z, f32 scale) {
    if (count < 2)
        return;
    f32 spacing = self->config->spacing * scale;
    f32 spacing_sq = spacing * spacing;
    for (u32 i = 1; i < count; ++i) {
        CVector2D pos = points->data[i - 1].xy;
        CVector2D end = points->data[i].xy;
        f32 dx = end.x - pos.x;
        f32 dy = end.y - pos.y;
        f32 len = __builtin_sqrtf(dx * dx + dy * dy);
        if (len != 0.0f) {
            dx /= len;
            dy /= len;
        }
        f32 angle = atan2f(dx, 0.0f - dy) - 1.5707964f;
        AddPoint_l802df0(positions, rotations, pos, z, angle);
        while (DistanceSq_l802df0(pos, end) > spacing_sq) {
            pos.x = spacing * dx + pos.x;
            pos.y = spacing * dy + pos.y;
            AddPoint_l802df0(positions, rotations, pos, z, angle);
        }
    }
}
