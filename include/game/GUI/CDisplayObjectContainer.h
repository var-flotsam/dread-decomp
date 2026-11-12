#pragma once

// CDisplayObjectContainer: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2748,
// 0x71011d6a04-0x71011dcc4c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkGuiEvent_b49;
struct UnkDisplayObjectContainer_ovb0032;
struct UnkDisplayObject_ovb0032;
struct UnkGuiButtonFocus_ovb0000;
struct UnkNewGameScreen;
struct UnkPanel_71011dfeb0;
struct UnkSmArg_A6_b62;
struct UnkSmBase_A6_b62;
void fn_71011d9b3c(UnkPanel_71011dfeb0* self);  // The base composition's set-up.
void fn_71011d9a44(UnkSmBase_A6_b62*, UnkSmArg_A6_b62*);
void fn_71011d8fe4(UnkNewGameScreen* self, f32 dt);
UnkDisplayObject_ovb0032* fn_71011d932c(UnkDisplayObjectContainer_ovb0032* self, UnkDisplayObject_ovb0032* child);
void fn_71011d9ab4(UnkGuiButtonFocus_ovb0000* self);
void fn_71011d9e90(void* a, void* b, void* c, bool d, bool e, bool f);
extern char g_unk_7172a0f120[] __attribute__((visibility("hidden")));
void fn_71011d99e4(UnkGuiDisplayObject* self, UnkGuiEvent_b49* event);
