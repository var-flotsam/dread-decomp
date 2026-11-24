// CFloatingPropActingComponent unit. fn_71007e63b8: after the base step (fn_71010c4228), keeps at
// +0x40 the weak handle of the object fn_71007e4924 finds for the owner (or nothing), then runs
// fn_71007e64d0. Names and layouts are placeholders.
#include "game/CCharClassEventScenarioComponent.h"
#include "game/CComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkParent;

struct UnkSelf_fp41 {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[0x8];
    UnkWeakHandle* handle;  // 0x40
};

void CFloatingPropActingComponent_PlayAnimation(UnkSelf_fp41* self);

void CFloatingPropActingComponent_FindTargetAndPlay(UnkSelf_fp41* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    if (UnkWeakTarget* target = fn_71007e4924(self->owner))
        AssignRef(&self->handle, GetWeakHandle(target));
    CFloatingPropActingComponent_PlayAnimation(self);
}
