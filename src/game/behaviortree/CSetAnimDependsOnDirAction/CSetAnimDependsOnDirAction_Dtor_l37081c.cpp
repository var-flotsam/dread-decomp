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
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;
    CStrId field_0x48;
    CStrId field_0x50;
    CStrId field_0x58;
    CStrId field_0x60;
    u8 pad_0x68[0x28];
    CStrId field_0x90;
    u8 field_0x98[0x148];
};
static_assert(__builtin_offsetof(UnkSetAnimDependsOnDirAction, field_0x90) == 0x90, "layout");

void fn_710037081c(UnkSetAnimDependsOnDirAction* self) {
    self->vtable = &g_unk_7101a24d08[2];
    CRntString* str = reinterpret_cast<CRntString*>(self->field_0x98);
    fn_710007ed40(str);
    fn_710007ecd4(str);
    self->field_0x90.~CStrId();
    self->field_0x60.~CStrId();
    self->field_0x58.~CStrId();
    self->field_0x50.~CStrId();
    self->field_0x48.~CStrId();
    self->field_0x40.~CStrId();
    self->field_0x38.~CStrId();
    self->field_0x30.~CStrId();
    fn_7100393ea0(self);
}
