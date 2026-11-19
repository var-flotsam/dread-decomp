#pragma once

// SGUISoundEventsSelector: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2625,
// 0x7100fc87ec-0x7100fcd378). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

struct UnkStateKey;

extern void* g_unk_7101cabad0[];  // The base class's vtable.
extern void* g_unk_7101caba40[];  // The selector's vtable.

struct UnkStateSink;  // The owner of the GUI's sound selector (g_unk_717275c380).
struct UnkStateKey;   // What names the event: the callers pass the address of a reflected type object.
extern UnkStateKey g_unk_71729f0188;  // The focus sound's event.
// Plays the sound the selector picks for the event `key` names, for the object `state`; the empty handle
// (no reference, serial -1) when it picks none.
UnkSoundHandle fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);
namespace unk_static_init {
struct UnkEnumType;  // game_5821e4/UnkStaticInit.h: a reflected enum type object
}  // namespace unk_static_init
extern unk_static_init::UnkEnumType g_unk_71729f02b0;  // The HUD ammo counters' fill sound event.
// Sound events the menus play (the unit's statics, built by fn_7100fcbf50).
extern unk_static_init::UnkEnumType g_unk_71729f01a0;
extern unk_static_init::UnkEnumType g_unk_71729f01a8;
extern unk_static_init::UnkEnumType g_unk_71729f01b0;
extern unk_static_init::UnkEnumType g_unk_71729f01d0;
extern UnkStateKey g_unk_71729f0178;
