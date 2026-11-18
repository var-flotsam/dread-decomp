#pragma once

// CButton: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2745,
// 0x71011be0d0-0x71011c1728). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkButtonClassData_l1c1530;
struct UnkGuiButton;
struct UnkGuiButtonIds;
struct UnkMissionLogButton_ovb0079;
struct UnkRumbleTunableButton_ovb0085;
UnkGuiDisplayObject* fn_71011c08dc(UnkMissionLogButton_ovb0079* self);
void fn_71011bf0ec(UnkGuiButton* self);  // CButton's constructor.
void fn_71011bf1a4(char* self);
void fn_71011bfd28(UnkRumbleTunableButton_ovb0085* self);
void fn_71011c14e0();  // The bound callback.
extern char g_unk_7101cb5908[];  // CButton's vtable.
void fn_71011c0158(UnkGuiDisplayObject* self);
// Selects what the button's label for `state` (0 to 3) names.
void fn_71011bf744(UnkGuiButtonIds* button, u32 state);
extern UnkButtonClassData_l1c1530 g_unk_7172a0d340 __attribute__((visibility("hidden")));
