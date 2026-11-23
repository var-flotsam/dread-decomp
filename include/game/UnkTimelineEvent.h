#pragma once

#include "game/nn/time.h"
#include "types.h"

// A timeline event (base::global::timeline; as src/game/CCutscene.cpp's): its class, then the
// frames it spans.
// A timeline event (base::global::timeline): its class, then the frames it spans.
struct UnkTimelineEvent {
    void** vtable;
    const base::reflection::CClass* cls;
    f32 start;
    f32 end;
};
