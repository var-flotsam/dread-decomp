#pragma once

// Unit 108's nearest-edge query on a 2D polygon (fn_7100117ba0, tools/tu_map.py unit 108), as the callers
// that pass it callbacks declare it: the edge nearest `point`, the parameter along it, the closest point and
// its distance, with an optional point filter and an edge visitor. Names and layouts are placeholders.
#include <functional>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

struct UnkPolygon2D;
using UnkPointFilter = std::function<bool(const base::math::CVector2D&)>;
using UnkEdgeVisitor = std::function<void(u32, const UnkSegment2D&)>;

void fn_7100117ba0(const UnkPolygon2D* polygon, const base::math::CVector2D* point, s32* edge, f32* t,
                   base::math::CVector2D* closest, f32* distance, bool flag, UnkPointFilter filter,
                   UnkEdgeVisitor visit);
