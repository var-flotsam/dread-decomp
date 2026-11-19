#pragma once

// The bounding-volume tree's sweep test (unit 2510, game/logic/collision/UnkBvhTree.h has the layouts):
// whether the shape placed by `xf` and moved by `delta` hits anything on the layers of `mask`.
// Names are placeholders.
#include "types.h"

struct UnkTree;
struct UnkShape;
struct UnkMtx44;
struct Vec2;
bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);
