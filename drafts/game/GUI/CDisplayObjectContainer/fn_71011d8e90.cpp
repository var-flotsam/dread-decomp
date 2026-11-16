// GUI::CDisplayObjectContainer's unit: the container's destructor (fn_71011d8e90). It destroys the child
// list (a pool list, base/UnkCloneHelpers.h) and the name, then runs GUI::CDisplayObject's destructor.
// Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/GUI/CMissionLog.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "types.h"

extern char g_unk_7101cb5dd0[];

struct UnkChildPoolListTag_l1d8e90;
template <>
struct base::UnkPoolListTraits<UnkChildPoolListTag_l1d8e90> {
    static char* vtable() { return g_unk_7101a9d490; }
    static char* base_vtable() { return g_unk_7101a9d4b0; }
};
// Nodes of {child, next}.
using UnkChildPoolList_l1d8e90 = base::UnkPoolList<UnkChildPoolListTag_l1d8e90, 0x8, 0x10>;

struct UnkGuiContainer_l1d8e90 {
    char* vtable;
    u8 pad_0x8[0x188];
    base::global::CRntString name;  // +0x190
    u8 pad_name[0x30 - sizeof(base::global::CRntString)];
    UnkChildPoolList_l1d8e90 children;  // +0x1c0
};
static_assert(__builtin_offsetof(UnkGuiContainer_l1d8e90, children) == 0x1c0, "layout");

void fn_71011d8e90(UnkGuiDisplayObject* object) {
    UnkGuiContainer_l1d8e90* self = reinterpret_cast<UnkGuiContainer_l1d8e90*>(object);
    self->vtable = g_unk_7101cb5dd0 + 0x10;
    self->children.~UnkPoolList();
    fn_710007ecd4(&self->name);
    fn_71011ccfb4(reinterpret_cast<char*>(self));
}