#pragma once

// The shared shape tester's result (fn_7100d572f8, include/game/UnkTU2509.h): returned in memory, as it is not
// trivially copyable. The tester writes only the first word, whose bit 0 is the hit. 8 bytes: fn_7101126c34 keeps
// it at sp+0 beside a point at sp+8 that is live across the call (at most 8), and fn_71007e98c8's frame gives it
// the slot of a dead 12-byte array ahead of an 8-byte point (LLVM's stack colouring takes the larger first, the
// earlier on a tie: at least 8). Names and layout are placeholders.
#include "types.h"

struct UnkShapeHit_A5_b68 {
    u32 flags;  // bit 0: hit
    u8 pad_0x4[0x4];
    UnkShapeHit_A5_b68(const UnkShapeHit_A5_b68& other);
};
