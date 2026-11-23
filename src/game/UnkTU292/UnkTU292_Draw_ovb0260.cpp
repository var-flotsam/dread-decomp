// Unit 292: hands an item list's handles to the renderer and draws with a position copy, then finishes the
// renderer. Names and layouts are placeholders.
#include "game/UnkDrawItem_b260.h"
#include "game/UnkDrawParent_b260.h"
#include "game/UnkDrawer_b260.h"
#include "game/UnkItemList_b260.h"
#include "game/UnkRenderer_b260.h"
#include "game/UnkTU167.h"
#include "game/UnkXformNode.h"
#include "types.h"

void fn_710020e0c0(UnkDrawer_b260* self, UnkItemList_b260* list, u64 a2, u64 a3, u64 a4, void* arg, u64 a6, u64 a7,
                   bool flag_a, bool flag_b, const UnkVec3f* vec, f32 t) {
    u32 zero = 0;
    u32 n;
    void** handles = fn_7100150b50(&zero);
    void* first;
    u32 count;
    if (list) {
        n = 0;
        UnkDrawItem_b260** items = list->GetItems(&n);
        u64 i = 0;
        if (n) {
            do {
                UnkDrawItem_b260* item = items[i];
                item->Prepare(1);
                handles[i] = item->GetHandle();
                ++i;
            } while (i < n);
        }
        count = (u32)i;
        first = self->parent->field_0x108;
    } else {
        first = nullptr;
        count = 0;
    }
    self->renderer->SetHandles(handles, count);
    UnkRenderer_b260* r = self->renderer;
    UnkVec3f v = *vec;
    f32 tt = t;
    u32 fa = flag_a, fb = flag_b;
    r->Draw(first, 0, arg, tt, fa, fb, &v);
    self->renderer->Finish();
}
