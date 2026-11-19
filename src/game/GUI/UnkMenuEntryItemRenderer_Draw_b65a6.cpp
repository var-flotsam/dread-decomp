// The debug, legend and scroll menu entry item renderers' units (GUI::CDebugMenuEntryItemRenderer,
// GUI::CLegendMenuEntryItemRenderer, GUI::CScrollMenuEntryItemRenderer): pass their arguments on to
// fn_71011d9e90 when the last flag is set, else to fn_71011cff5c. Names are placeholders.
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkTU2747.h"
#include "types.h"

void* fn_7100affb94(UnkDisplayContainer_l1d9e90* self, u32 input, void* event, bool d, bool all, bool f) {
    if (f)
        return fn_71011d9e90(self, input, event, d, all, true);
    return fn_71011cff5c(self, input, event, d, all, false);
}

void* fn_7100b33814(UnkDisplayContainer_l1d9e90* self, u32 input, void* event, bool d, bool all, bool f) {
    if (f)
        return fn_71011d9e90(self, input, event, d, all, true);
    return fn_71011cff5c(self, input, event, d, all, false);
}

void* fn_7100b75ae8(UnkDisplayContainer_l1d9e90* self, u32 input, void* event, bool d, bool all, bool f) {
    if (f)
        return fn_71011d9e90(self, input, event, d, all, true);
    return fn_71011cff5c(self, input, event, d, all, false);
}
