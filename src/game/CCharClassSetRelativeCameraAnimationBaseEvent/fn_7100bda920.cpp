// Describes an object's settings in a property bag and hands the bag to a manager-owned
// receiver. Layouts are placeholders.
#include "../../game_s7/UnkPropBagS7.h"
#include "game/UnkGameManager.h"

using namespace unk_prop_bag;

struct UnkHandle_7100bda920 {
    void* ptr;
};

struct UnkObj_7100bda920 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void* vf_0x20(UnkHandle_7100bda920* handle);
    u8 unk_0x8[0x10];
    bool bRawChange;
    f32 fInitInterpTime;
    f32 fExitInterpTime;
    UnkName* field_0x28;
    bool bExitInterpStoringValues;
    UnkName* field_0x38;
    UnkName* field_0x40;
    bool bHideGUI;
    bool bInitialInterp;
    bool bInvertSide;
    bool bInvertAng;
    u32 iIniFrame;
    f32 fInitInterpFactor;
    bool bExitWantsToForceEntitiesInFOV;
    bool bResetInterpOnPossess;
    f32 fDuration;
    bool bReplaceCurrentCameraAnimation;
    UnkName* field_0x60;
    bool bUpdateGoalParamsOnExitInterpolation;
    bool bLoopAnim;
    bool bUseRelativeAnimTime;
    bool bUseBoundaries;
};

struct UnkReceiver;
void fn_71010e560c(UnkReceiver* receiver, UnkPropBag* bag, void* target, void* info, const UnkPair& a,
                   const UnkPair& b);

void fn_7100bda920(UnkObj_7100bda920* self, UnkHandle_7100bda920* handle) {
    if (!handle->ptr)
        return;
    UnkReceiver* receiver = static_cast<UnkReceiver*>(fn_7100defcec(g_unk_7101d47310));
    if (!receiver)
        return;
    UnkPropBag bag;
    fn_710033187c(&bag, Key(0xeb0), self->fDuration);
    fn_710050f4ac(&bag, Key(0xf58), self->bExitInterpStoringValues);
    fn_710033187c(&bag, Key(0xf60), self->fExitInterpTime);
    fn_710050e4dc(&bag, Key(0xf70), UseName(self->field_0x28));
    fn_710050f4ac(&bag, Key(0xf78), self->bExitWantsToForceEntitiesInFOV);
    fn_710050f4ac(&bag, Key(0x1370), self->bHideGUI);
    fn_7100aa3b9c(&bag, Key(0x1448), self->iIniFrame);
    fn_710033187c(&bag, Key(0x1488), self->fInitInterpFactor);
    fn_710033187c(&bag, Key(0x1490), self->fInitInterpTime);
    fn_710050f4ac(&bag, Key(0x14e8), self->bInvertAng);
    fn_710050f4ac(&bag, Key(0x14f0), self->bInvertSide);
    fn_710050f4ac(&bag, Key(0x1d40), !self->bInitialInterp || self->bRawChange);
    fn_710050f4ac(&bag, Key(0x1dd0), self->bReplaceCurrentCameraAnimation);
    fn_710050f4ac(&bag, Key(0x1de0), self->bResetInterpOnPossess);
    fn_710050e4dc(&bag, Key(0x21d0), UseName(self->field_0x38));
    fn_710050e4dc(&bag, Key(0xf10), UseName(self->field_0x40));
    fn_710050f4ac(&bag, Key(0x2458), self->bUpdateGoalParamsOnExitInterpolation);
    fn_710050f4ac(&bag, Key(0x1680), self->bLoopAnim);
    fn_710050f4ac(&bag, Key(0x510), self->bUseRelativeAnimTime);
    fn_710050f4ac(&bag, Key(0x508), self->bUseBoundaries);
    fn_710050e4dc(&bag, Key(0x7d8), UseName(self->field_0x60));
    fn_71010e560c(receiver, &bag, handle->ptr, self->vf_0x20(handle), UnkPair(), UnkPair());
}
