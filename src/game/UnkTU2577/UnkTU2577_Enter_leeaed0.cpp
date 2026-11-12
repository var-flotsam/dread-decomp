// CSubAreaManager's unit (tools/tu_map.py unit 2577): fn_7100eeaed0 enters the current sub-area. It marks the
// change as running and the scenario's flag clear; in state 3 it stops the effect player's fade (starting a
// short one at the origin when none runs), disables the player's component and takes the player as the follow
// actor, otherwise it hands the sub-area's settings to the environment manager and the camera and applies the
// full mask to it; then it collects the sub-area's two render lists, steps the change once with no time and,
// outside state 3, reports the previous and the current sub-area's ids. The manager's view is that of the
// unit's other files (src/game/UnkTU2577/UnkTU2577_Fade_ovb0190.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CActorComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkMask.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2611.h"
#include "game/UnkTU2656.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

// A sub-area (a placeholder for the fields used here): two ids.
struct UnkSubArea {
    CStrId field_0x0;
    u8 pad_0x8[0x48];
    CStrId field_0x50;
};
static inline const CStrId& FirstId(const UnkSubArea* area) { return area ? area->field_0x0 : g_unk_7101d470e0; }
static inline const CStrId& SecondId(const UnkSubArea* area) { return area ? area->field_0x50 : g_unk_7101d470e0; }

struct UnkScenarioSub {
    u8 pad_0x0[0x58];
    bool field_0x58;
};
struct UnkScenario {
    u8 pad_0x0[0x820];
    UnkScenarioSub* field_0x820;
};

struct UnkSubareaRenderOwner;
struct UnkSubareaRenderSetup;
struct UnkSubareaRenderOut {
    void* data;
    u32 count;
};
void fn_7100facb08(UnkSubareaRenderOwner* self, s32 kind, UnkSubareaRenderSetup* setup, bool flag,
                   UnkSubareaRenderOut* out);

struct UnkSubAreaMgr_b190 {
    u8 pad_0x0[0x108];
    u32 field_0x108;
    u8 pad_0x10c[4];
    u32 field_0x110;
    u8 pad_0x114[0x2dc];
    UnkSubArea* field_0x3f0;
    UnkSubArea* field_0x3f8;
    u8 pad_0x400[0x538];
    UnkSubareaRenderOut field_0x938;
    u8 pad_0x948[0x18];
    UnkSubareaRenderOut field_0x960;
    u8 pad_0x970[0x18];
    f32 field_0x988;
    u8 pad_0x98c[0x154];
    bool field_0xae0;
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x938) == 0x938, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x960) == 0x960, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x988) == 0x988, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0xae0) == 0xae0, "layout");

void fn_7100eeaed0(UnkSubAreaMgr_b190* self) {
    self->field_0x110 = 1;
    UnkGameManager* gm = g_unk_7101d47310;
    reinterpret_cast<UnkScenario*>(gm->field_0x608)->field_0x820->field_0x58 = false;
    if (self->field_0x108 == 3) {
        if (self->field_0xae0) {
            void* effects = gm->field_0x1040;
            fn_710105383c(effects);
            if (!fn_7101053a0c(effects)) {
                UnkVec3f zero(0.0f, 0.0f, 0.0f);
                fn_7101053850(effects, reinterpret_cast<UnkVec*>(&zero), 0, 0.2f, 1.0f);
            }
        }
        fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(fn_7100defcec(gm)), false);
        fn_7100ee6ae4(reinterpret_cast<CSubAreaManager*>(self));
    } else {
        fn_7100eea4c8(UnkSubAreaTag(), self->field_0x3f0, false, false, UnkMask(0x1c0));
        fn_7100eea618(UnkSubAreaTag(), self->field_0x3f0, false);
        fn_7100fac120(self->field_0x3f0, UnkMaskOf<UnkSubAreaMaskTag>::All(), true, false, self->field_0x3f8, false);
    }
    self->field_0x960.count = 0;
    self->field_0x938.count = 0;
    if (self->field_0x3f0) {
        fn_7100facb08(reinterpret_cast<UnkSubareaRenderOwner*>(self->field_0x3f0), 1,
                      reinterpret_cast<UnkSubareaRenderSetup*>(self->field_0x3f8), false, &self->field_0x960);
        fn_7100facb08(reinterpret_cast<UnkSubareaRenderOwner*>(self->field_0x3f0), 1,
                      reinterpret_cast<UnkSubareaRenderSetup*>(self->field_0x3f8), true, &self->field_0x938);
    }
    self->field_0x988 = 0.0f;
    fn_7100ee4d34(reinterpret_cast<CSubAreaManager*>(self), 0.0f);
    if (self->field_0x108 != 3)
        fn_7100eeaa48(reinterpret_cast<CSubAreaManager*>(self), FirstId(self->field_0x3f8), SecondId(self->field_0x3f8),
                      FirstId(self->field_0x3f0), SecondId(self->field_0x3f0), false);
}
