#pragma once

// CCameraFX_Shake: functions of this translation unit that other files call (tools/tu_map.py unit 2701,
// 0x71010f3610-0x71010f41a0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkMotionShake_l0f1e24;
struct UnkCameraShake_ovb0108;
struct UnkShakePos_ovb0108;
// The constructor (fn_71010f3d20) is the class's own (reflected/CCameraFX_Shake.h).
class CCameraFX_Shake;
void fn_71010f3e94(CCameraFX_Shake* self);
void fn_71010f3eb0(UnkMotionShake_l0f1e24* self, void* camera, base::math::CVector3D* pos, base::math::CVector3D* target, void* arg4, void* arg5, f32 dt);
f32 fn_71010f3f20(UnkCameraShake_ovb0108* self, const UnkShakePos_ovb0108* pos, f32 scale);
