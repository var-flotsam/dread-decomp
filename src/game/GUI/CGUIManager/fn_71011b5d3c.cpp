// In the GUI manager's unit (unit 2744 of tools/tu_map.py): the display object a path names, from
// the root's menu (game/UnkGuiRoot.h). "[Root]" names the menu itself; any other path is looked up
// among its children (fn_71011d9508). Null without a menu.
#include "../../../game_s7/UnkGuiS7.h"
#include "base/global/UnkStringPool.h"
#include "game/UnkGuiRoot.h"

UnkGuiDisplayObject* fn_71011b5d3c(UnkGuiRoot* root, const char* const* path) {
    if (!root->menu)
        return nullptr;
    if (fn_710007f240(reinterpret_cast<char*>(const_cast<const char**>(path)), "[Root]"))
        return unk_gui::fn_71011d9508(root->menu, path);
    return root->menu;
}
