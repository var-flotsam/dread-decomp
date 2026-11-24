// GUI::CMapMenuController's unit (tools/tu_map.py unit 1689): sets the teleport prompt's two button
// labels (select, confirm or none; cancel) and refreshes both buttons. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiHandle.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CStrId;

// The map menu controller's members this function reads.
struct UnkMapMenuTeleport_lb44344 {
    u8 pad_0x0[0x338];
    UnkGuiHandleObj* handle_0x338;  // 0x338: the accept button
    UnkGuiHandleObj* handle_0x340;  // 0x340: the cancel button
    u8 pad_0x348[0x160];
    CStrId id;  // 0x4a8: the selected teleporter
    u8 pad_0x4b0[0x440];
    base::global::UnkStringData** known_ids;  // 0x8f0
    u32 known_count;                          // 0x8f8
};
static_assert(__builtin_offsetof(UnkMapMenuTeleport_lb44344, known_ids) == 0x8f0, "layout");

static inline UnkGuiDisplayObject* TargetOf(UnkGuiHandleObj* handle) {
    return handle ? handle->target : nullptr;
}

// The selected id's position among the known teleporters, -1 when it is not one.
static inline s32 FindSelected(const UnkMapMenuTeleport_lb44344* self) {
    for (u32 i = 0; i < self->known_count; ++i) {
        if (self->known_ids[i] == self->id.data)
            return i;
    }
    return -1;
}

// The label's text setter is given the label's string id.
static inline void SetLabel(UnkGuiDisplayObject* object, const char* text) {
    CStrId label(text, true);
    fn_71011ecfbc(object, reinterpret_cast<const char32_t*>(&label));
}

void UpdateMapMenuTeleportButtonLabels(UnkMapMenuTeleport_lb44344* self) {
    if (FindSelected(self) >= 0)
        SetLabel(TargetOf(self->handle_0x338), "#GUI_MAPMENU_LABEL_CONFIRM_TELEPORT");
    else if (self->known_count != 0)
        SetLabel(TargetOf(self->handle_0x338), "#GUI_MAPMENU_LABEL_SELECT_TELEPORT");
    else
        SetLabel(TargetOf(self->handle_0x338), "#GUI_MAPMENU_LABEL_NO_TELEPORT");
    fn_71011cd5a0(TargetOf(self->handle_0x338));
    SetLabel(TargetOf(self->handle_0x340), "#GUI_MAPMENU_LABEL_CANCEL_TELEPORT");
    fn_71011cd5a0(TargetOf(self->handle_0x340));
}
