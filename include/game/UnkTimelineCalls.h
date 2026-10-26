#pragma once

// The calls on the timeline component (game/UnkTimelineComponent.h) that need only a pointer to
// it, so a header that calls them does not have to pull in the actor.

#include "types.h"

struct UnkTimelineComponent;

// Sets a value for an id (-1 = none): on the timed list when the delay is positive or about
// zero, else now, or queued while the table is locked; `unique` ignores unknown ids on the
// immediate path. Defined in src/game_hi/fn_7100a2cb7c.cpp.
void fn_7100a2cb7c(UnkTimelineComponent* self, const u64* id, const u32* value, bool unique, f32 delay);
