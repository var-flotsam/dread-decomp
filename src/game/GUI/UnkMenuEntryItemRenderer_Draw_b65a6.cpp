// The debug, legend and scroll menu entry item renderers' units (GUI::CDebugMenuEntryItemRenderer,
// GUI::CLegendMenuEntryItemRenderer, GUI::CScrollMenuEntryItemRenderer): pass their arguments on to
// fn_71011d9e90 when the last flag is set, else to fn_71011cff5c. Names are placeholders.
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkTU2747.h"
#include "types.h"

void fn_7100affb94(void* a, void* b, void* c, bool d, bool e, bool f) {
    if (f)
        fn_71011d9e90(a, b, c, d, e, true);
    else
        fn_71011cff5c(a, b, c, d, e, false);
}

void fn_7100b33814(void* a, void* b, void* c, bool d, bool e, bool f) {
    if (f)
        fn_71011d9e90(a, b, c, d, e, true);
    else
        fn_71011cff5c(a, b, c, d, e, false);
}

void fn_7100b75ae8(void* a, void* b, void* c, bool d, bool e, bool f) {
    if (f)
        fn_71011d9e90(a, b, c, d, e, true);
    else
        fn_71011cff5c(a, b, c, d, e, false);
}
