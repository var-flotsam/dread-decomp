// CCharClassSetRelativeCameraAnimationBaseEvent's unit (tools/tu_map.py): its constructor, on the
// event base (fn_7100067ea4): the vtable, a file-path id built from the empty id's string
// (refl_GlobalRntString), a cleared flag, a cleared word and a float of 1, three string ids built
// from "" with a cleared flag between, flags and floats (5, -1), a fourth string id and a last set
// flag. Names and the layout are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/ReflectedAttributes.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern char g_unk_7101aa1be8[];

struct UnkSetRelativeCameraAnimationBaseEvent_b42a3 {
    void* vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId path_0x10;
    bool bRawChange;
    u32 field_0x1c;
    f32 fExitInterpTime;
    CStrId id_0x28;
    bool bExitInterpStoringValues;
    CStrId id_0x38;
    CStrId id_0x40;
    bool bHideGUI;
    bool bInitialInterp;
    u16 field_0x4a;
    u32 iIniFrame;
    f32 fInitInterpFactor;
    bool bExitWantsToForceEntitiesInFOV;
    bool bResetInterpOnPossess;
    f32 fDuration;
    bool bReplaceCurrentCameraAnimation;
    CStrId id_0x60;
    bool bUpdateGoalParamsOnExitInterpolation;
    bool bLoopAnim;
    bool bUseRelativeAnimTime;
    bool bUseBoundaries;
};

void fn_7100bdb3b4(UnkSetRelativeCameraAnimationBaseEvent_b42a3* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa1be8 + 0x10;
    fn_710001a270(&self->path_0x10, refl_GlobalRntString(), true);
    self->bRawChange = false;
    self->field_0x1c = 0;
    self->fExitInterpTime = 1.0f;
    new (&self->id_0x28) CStrId("", true);
    self->bExitInterpStoringValues = false;
    new (&self->id_0x38) CStrId("", true);
    new (&self->id_0x40) CStrId("", true);
    self->bHideGUI = true;
    self->bInitialInterp = true;
    self->field_0x4a = 0;
    self->iIniFrame = 0;
    self->fInitInterpFactor = 5.0f;
    self->bExitWantsToForceEntitiesInFOV = false;
    self->bResetInterpOnPossess = false;
    self->fDuration = -1.0f;
    self->bReplaceCurrentCameraAnimation = true;
    new (&self->id_0x60) CStrId("", true);
    self->bUpdateGoalParamsOnExitInterpolation = false;
    self->bLoopAnim = false;
    self->bUseRelativeAnimTime = false;
    self->bUseBoundaries = true;
}
