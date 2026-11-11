#pragma once

// CButton: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2745,
// 0x71011be0d0-0x71011c1728). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGuiButton;
struct UnkGuiObj;
struct UnkMissionLogButton_ovb0079;
struct UnkRumbleTunableButton_ovb0085;
UnkGuiObj* fn_71011c08dc(UnkMissionLogButton_ovb0079* self);
void fn_71011bf0ec(UnkGuiButton* self);  // CButton's constructor.
void fn_71011bf1a4(char* self);
void fn_71011bfd28(UnkRumbleTunableButton_ovb0085* self);
void fn_71011c14e0();  // The bound callback.
extern char g_unk_7101cb5908[];  // CButton's vtable.
