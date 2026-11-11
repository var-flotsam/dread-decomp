#pragma once

// CCameraCtrl: functions of this translation unit that other files call (tools/tu_map.py unit 2693,
// 0x71010ecae0-0x71010eee90). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkArg_fc;
struct UnkBase_7100763ed4;
struct UnkChozoCommanderDropCameraCtrl;
struct UnkFixedCamera_fc;
struct UnkGrabCameraCtrl;
void fn_71010ed044(UnkBase_7100763ed4*);
void fn_71010ed98c(UnkFixedCamera_fc* self, UnkArg_fc* arg, u32 flag);
void fn_71010edd64(UnkGrabCameraCtrl* self);
void fn_71010ee17c(UnkChozoCommanderDropCameraCtrl* self);
