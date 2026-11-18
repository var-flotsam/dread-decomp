// GUI::CLabel unit (tools/tu_map.py, unit 2752): after the display object's handler (fn_71011cf70c), when a page
// is pending (+0x314 positive) it becomes the current one (+0x310); on a change the object's vf_0xa0 runs once
// (+0x184). Then the range at +0x318 is reset to (-1, 1) and the callbacks at +0x198 are told `true`.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;

struct UnkLabel_l1ee87c : UnkGuiDisplayObject {
    UnkDeferredSet<UnkDeferredCallback<bool>> on_change;  // 0x198
    u8 pad_0x218[0xf8];
    s32 current;  // 0x310
    s32 pending;  // 0x314
    f32 range_min;  // 0x318
    f32 range_max;  // 0x31c
};
static_assert(__builtin_offsetof(UnkLabel_l1ee87c, on_change) == 0x198, "layout");
static_assert(__builtin_offsetof(UnkLabel_l1ee87c, range_min) == 0x318, "layout");

void fn_71011ee87c(UnkLabel_l1ee87c* self) {
    fn_71011cf70c(reinterpret_cast<UnkGallery_A5_b75*>(self));
    s32 pending = self->pending;
    if (pending < 1)
        return;
    if (self->current != pending) {
        self->current = pending;
        if (!self->field_0x184) {
            self->field_0x184 = true;
            self->vf_0xa0();
        }
    }
    self->range_min = -1.0f;
    self->range_max = 1.0f;
    bool arg = true;
    fn_71000303c0(&self->on_change, &arg);
}
