#pragma once

#include "game/UnkTU108.h"
#include "game/UnkTilePoint.h"

struct UnkTileOutline {
    base::global::CRntVector<UnkTilePoint> points;
    f32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    base::math::CVector2D min;
    base::math::CVector2D max;
    u32 field_0x40;

    UnkTileOutline()
        : points(7), field_0x28(0.0f), field_0x2c(false), field_0x2d(true), min{3.4028235e38f, 3.4028235e38f},
          max{-3.4028235e38f, -3.4028235e38f}, field_0x40(0) {}
    ~UnkTileOutline() { points.Free(); }
};
