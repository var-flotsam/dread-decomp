// CCharClassHideScenarioItemByNameEvent (unit 2257 of tools/tu_map.py): hides or shows
// the target item (the word at +0x30 of the object at +0x10) by the event's two flags (+0x18, +0x19) through
// the render manager's fn_7100219c5c. Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

struct UnkHideScenarioItemTarget {
    u8 pad_0x0[0x30];
    void* item;
};

struct UnkHideScenarioItemEvent {
    u8 pad_0x0[0x10];
    UnkHideScenarioItemTarget* target;
    bool hide;
    bool flag_0x19;
};

void fn_7100cc4384(UnkHideScenarioItemEvent* self) {
    fn_7100219c5c(g_unk_7101d47300, self->hide, self->target->item, 0, self->flag_0x19);
}
