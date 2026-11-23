#pragma once

#include "game/UnkActorLink_l7a58c0.h"
#include "types.h"

struct UnkCutsceneActor_l7a58c0;

// The object an entry's handle refers to: the actor at +0x10.
struct UnkActorRef_l7a58c0 : base::UnkRefCounted {
    UnkCutsceneActor_l7a58c0* actor;  // 0x10
};
