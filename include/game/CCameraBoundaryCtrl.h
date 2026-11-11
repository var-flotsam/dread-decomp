#pragma once

// CCameraBoundaryCtrl: functions of this translation unit that other files call (tools/tu_map.py unit 1169,
// 0x7100763c78-0x7100767040). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct CSynchronizedBlockCameraCtrl;
struct UnkAnimatedCameraCtrl_b0101;
struct UnkBossCameraCtrl_b49a3;
struct UnkCamVec3_b0101;
struct UnkPureFollowCameraCtrl;
struct UnkStruct_7100763ed4;
void fn_7100763ed4(UnkStruct_7100763ed4* self);
void fn_7100763f74(UnkBossCameraCtrl_b49a3* self);
void fn_7100763f9c(CSynchronizedBlockCameraCtrl*, f32);
void fn_7100764388(UnkPureFollowCameraCtrl* self);
void fn_7100764a50(UnkAnimatedCameraCtrl_b0101* self, UnkCamVec3_b0101* from, UnkCamVec3_b0101* to, bool reset);
