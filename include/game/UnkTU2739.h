#pragma once

// UnkTU2739: functions of this translation unit that other files call (tools/tu_map.py unit 2739,
// 0x71011a7f34-0x71011abbd0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkFxCache;
struct UnkFxObject_ovb0041;
struct UnkModelManager;
struct UnkQueueOwner;
void fn_71011a9980(UnkQueueOwner* self);

// Defined in game_hi with its own view of the manager (its container half).
bool fn_71011a91c4(UnkModelManager* self, void* handle, u32 id);

void fn_71011a9bb4(UnkModelManager* self, UnkFxObject_ovb0041* fx);
void fn_71011aa820(UnkModelManager* self);
void fn_71011aa5a4(UnkFxCache* cache, const base::global::CFilePathStrId* path);
