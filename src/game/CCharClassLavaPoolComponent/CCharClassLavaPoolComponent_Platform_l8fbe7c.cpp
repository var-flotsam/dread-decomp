// CCharClassLavaPoolComponent unit. fn_71008fbe7c: the start; after the liquid pool's start it links the
// pool's platform: sets its level from the pool's position (vf_0x178) and keeps its weak handle at
// +0xc88, as the water pool does. Names and layouts are placeholders.
#include "game/CCharClassLavaPoolComponent.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/CLiquidSimulationComponent.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// Copied from src/game/CCharClassWaterPoolComponent/CCharClassWaterPoolComponent_Platform_ovb0086.cpp.
struct UnkWaterPoolPlatform_b0086 : UnkWeakTarget {};

struct UnkWaterPoolComponent_b0086;

struct UnkLavaPoolComponent_l8fbe7c {
    void** vtable;
    u8 pad_0x8[0x28];
    void* field_0x30;
    u8 pad_0x38[0xc88 - 0x38];
    UnkWeakHandle* platform;  // 0xc88

    UnkVec3f vf_0x178() { return reinterpret_cast<UnkVec3f (*)(UnkLavaPoolComponent_l8fbe7c*)>(vtable[0x178 / 8])(this); }
};

void fn_71008fbe7c(UnkLavaPoolComponent_l8fbe7c* self) {
    fn_7100900c44(reinterpret_cast<UnkWaterPoolComponent_b0086*>(self));
    UnkWaterPoolPlatform_b0086* platform = fn_71008fbfc0(self->field_0x30);
    if (!platform)
        return;
    fn_7100905bdc(platform, self->vf_0x178().x, true);
    AssignRef(&self->platform, GetWeakHandle(platform));
}
