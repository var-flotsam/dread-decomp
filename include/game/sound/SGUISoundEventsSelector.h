#pragma once

// SGUISoundEventsSelector: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2625,
// 0x7100fc87ec-0x7100fcd378). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/sound/UnkSoundHandle.h"
#include "types.h"
extern void* g_unk_7101cabad0[];  // The base class's vtable.
extern void* g_unk_7101caba40[];  // The selector's vtable.

struct UnkStateSink;  // The owner of the GUI's sound selector (g_unk_717275c380).
struct UnkStateKey;   // What names the event: the callers pass the address of a reflected type object.
// Plays the sound the selector picks for the event `key` names, for the object `state`; the empty handle
// (no reference, serial -1) when it picks none.
UnkSoundHandle fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);
