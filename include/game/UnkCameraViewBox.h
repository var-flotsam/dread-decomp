#pragma once

// The 2D bounds of the camera's view (the CCameraComponent unit fills it, the liquid simulation reads
// it). Names are placeholders.
#include "types.h"

struct UnkCameraViewBox {
    f32 minX, minY, maxX, maxY;
};

// The view's bounds (false when there is none).
bool fn_71010e53dc(void* camera, UnkCameraViewBox* box);
