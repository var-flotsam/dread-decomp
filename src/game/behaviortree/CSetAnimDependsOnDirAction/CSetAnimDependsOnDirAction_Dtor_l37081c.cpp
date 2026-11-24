// behaviortree::CSetAnimDependsOnDirAction unit: the node's destructor. Frees its string with
// inline storage, releases its eight string ids, then runs the tree node base's destructor.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/behaviortree/CBaseAction.h"
#include "game/behaviortree/CSetAnimDependsOnDirAction.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkSetAnimDependsOnDirAction {
    void** vtable;
    u8 pad_0x8[0x28];
    CStrId sInDirAnim;
    CStrId sInDirAnimProp;
    CStrId sNotInDirAnim;
    CStrId sNotInDirAnimProp;
    CStrId sInDirIdleAnim;
    CStrId sInDirIdleAnimProp;
    CStrId sTargetPos;
    u8 pad_0x68[0x28];
    CStrId sReturnActionDisp;
    u8 field_0x98[0x148];
};
static_assert(__builtin_offsetof(UnkSetAnimDependsOnDirAction, sReturnActionDisp) == 0x90, "layout");

void fn_710037081c(UnkSetAnimDependsOnDirAction* self) {
    self->vtable = &g_unk_7101a24d08[2];
    CRntString* str = reinterpret_cast<CRntString*>(self->field_0x98);
    CRntString_Clear(str);
    CRntString_Destroy(str);
    self->sReturnActionDisp.~CStrId();
    self->sTargetPos.~CStrId();
    self->sInDirIdleAnimProp.~CStrId();
    self->sInDirIdleAnim.~CStrId();
    self->sNotInDirAnimProp.~CStrId();
    self->sNotInDirAnim.~CStrId();
    self->sInDirAnimProp.~CStrId();
    self->sInDirAnim.~CStrId();
    fn_7100393ea0(self);
}
