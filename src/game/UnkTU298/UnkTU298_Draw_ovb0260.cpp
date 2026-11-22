// Unit 298: draws an item list's handles through the renderer (a copy of unit 292's draw without the
// per-item preparation). Names and layouts are placeholders.
#include "game/UnkDrawItem_b260.h"
#include "game/UnkDrawParent_b260.h"
#include "game/UnkItemList_b260.h"
#include "game/UnkRenderer_b260.h"
#include "game/UnkTU167.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkDrawer_b260 {
    u8 unk_0x0[8];
    UnkDrawParent_b260* parent;  // 0x8
    UnkRenderer_b260* renderer;  // 0x10
    u8 unk_0x18[8];
    u32 field_0x20;
};

void fn_7100210124(UnkDrawer_b260* self, UnkItemList_b260* list, u64 a2, u64 a3, u64 a4, void* a5, u64 a6, u64 a7,
                   bool flag_a, bool flag_b, const UnkVec3f* vec, f32 t) {
    void* first = self->parent->field_0x108;
    self->field_0x20 = 0;
    u32 zero = 0;
    u32 n;
    void** handles = fn_7100150b50(&zero);
    n = 0;
    UnkDrawItem_b260** items = list->GetItems(&n);
    u64 i = 0;
    if (n) {
        do {
            UnkDrawItem_b260* item = items[i];
            handles[i] = item->GetHandle();
            ++i;
        } while (i < n);
    }
    u32 count = (u32)i;
    self->renderer->SetHandles(handles, count);
    UnkRenderer_b260* r = self->renderer;
    UnkVec3f v = *vec;
    f32 tt = t;
    u32 fa = flag_a, fb = flag_b;
    r->Draw(first, a4, a5, tt, fa, fb, &v);
    self->renderer->Finish();
}
