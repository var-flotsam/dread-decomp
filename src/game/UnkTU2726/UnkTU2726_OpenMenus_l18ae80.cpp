// Unit 2726 (gui/scripts/msemenu_ingame.lua): loads the debug in-game menu composition and creates the
// in-game menu root, keeping weak handles to both, adding them to the GUI root's menu container and
// starting the in-game menu. Names and layouts are placeholders.

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/GUI/CIngameMenu.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiHandle.h"
#include "game/UnkGuiRoot.h"
#include "game_s2/UnkWeakHandle.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

struct UnkMenuStates;
struct UnkStateOwner;
void fn_7100b2e54c(UnkMenuStates* self);

// Loads the composition `name` from the script `path` with the properties in `bag`.
UnkGuiHandle fn_71011b6030(UnkGuiRoot* root, const CRntString& name, const CRntString& path, UnkPropBag* bag);
// Registers `object` with the root under `name`.
void fn_710118fd30(UnkGuiRoot* root, UnkWeakTarget* object, const CStrId& name, UnkPropBag* bag);
void fn_7100e02acc(UnkGameManager* manager, UnkGuiDisplayObject* object);

struct UnkMenuOwner {
    u8 pad_0x0[0xe8];
    UnkWeakTarget* field_0xe8;        // the in-game menu root
    UnkWeakHandle* field_0xf0;        // handle of the in-game menu root
    UnkWeakHandle* field_0xf8;        // handle of the debug menu
};
static_assert(__builtin_offsetof(UnkMenuOwner, field_0xf8) == 0xf8, "UnkMenuOwner placeholder layout");

static inline UnkGuiDisplayObject* Target(UnkWeakHandle* handle) {
    return handle ? reinterpret_cast<UnkGuiDisplayObject*>(handle->target) : nullptr;
}

void fn_710118ae80(UnkMenuOwner* self) {
    UnkPropBag bag;
    fn_710050f4ac(&bag, CStrId("Enabled", true), true);
    fn_710050f4ac(&bag, CStrId("Focusable", true), true);

    UnkGuiRoot* root = g_unk_717271e8f8;
    UnkGuiHandle debug_menu =
        fn_71011b6030(root, CRntString("DebugIngameMenu"), CRntString("gui/scripts/msemenu_ingame.lua"), &bag);
    if (UnkGuiDisplayObject* object = debug_menu.Get()) {
        UnkWeakHandle* handle = GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(object));
        AssignRef(&self->field_0xf8, handle);
        fn_71011cde44(Target(handle), 2);
        fn_71011d968c(g_unk_717271e8f8->menu, Target(self->field_0xf8));
    }

    UnkGuiRoot* root2 = g_unk_717271e8f8;
    UnkWeakTarget* menu_root;
    {
        CStrId name("IngameMenuRoot", true);
        menu_root = static_cast<UnkWeakTarget*>(fn_71000839a0(0x498, 0xc));
        fn_7100b2e54c(reinterpret_cast<UnkMenuStates*>(menu_root));
        if (menu_root)
            menu_root->vf_0x18();
        fn_710118fd30(root2, menu_root, name, &bag);
        asm("" : "+r"(menu_root));
    }
    if (menu_root) {
        UnkWeakHandle* handle = GetWeakHandle(menu_root);
        AssignRef(&self->field_0xf0, handle);
        fn_71011cde44(Target(handle), 2);
        fn_71011d968c(g_unk_717271e8f8->menu, Target(self->field_0xf0));
        UnkWeakTarget** current = &self->field_0xe8;
        *current = self->field_0xf0 ? self->field_0xf0->target : nullptr;
        fn_7100e02acc(g_unk_7101d47310, Target(self->field_0xf0));
        fn_7100b2eaac(reinterpret_cast<UnkStateOwner*>(Target(self->field_0xf0)), 1, true, true);
    }
}
