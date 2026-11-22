#pragma once

#include "types.h"

// The answer of fn_710081b920 (src/game/CGrappleBeamComponent/CGrappleBeamComponent_b50.cpp), here read as
// a position.
// The answer, built in place (not trivially copyable: returned in memory).
struct UnkGrappleAnswer_b50 {
    void* field_0x0;
    u32 field_0x8;

    UnkGrappleAnswer_b50() : field_0x0(nullptr), field_0x8(0) {}
    UnkGrappleAnswer_b50(const UnkGrappleAnswer_b50& other);
};
