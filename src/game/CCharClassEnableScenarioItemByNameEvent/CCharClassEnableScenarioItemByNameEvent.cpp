// CCharClassEnableScenarioItemByNameEvent (unit 2238 of tools/tu_map.py): switches the
// target item's render layer (the word at +0x30 of the object at +0x10) on or off by the event's flag (+0x18)
// through the render manager's fn_7100219ca4. Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

struct UnkScenarioItemTarget {
    u8 pad_0x0[0x30];
    void* item;
};

struct UnkEnableScenarioItemEvent {
    u8 pad_0x0[0x10];
    UnkScenarioItemTarget* target;
    bool enable;
};

void fn_7100cb948c(UnkEnableScenarioItemEvent* self) {
    fn_7100219ca4(g_unk_7101d47300, self->target->item, self->enable, 0);
}
