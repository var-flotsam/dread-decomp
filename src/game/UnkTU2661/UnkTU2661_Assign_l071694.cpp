// Unit 2661 (CCutscene::CTunableCutscene): the copy assignment of a base::global::CRntVector of 0x18-byte
// entries whose third word is a render item, taken out of the render manager (fn_71002199d8) when the entry
// is destroyed. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkRenderEntry_l071694 {
    u64 field_0x0;
    u64 field_0x8;
    UnkItemBase_71011a91c4* item;  // 0x10

    ~UnkRenderEntry_l071694() {
        fn_71002199d8(g_unk_7101d47300, item, 0);
        item = nullptr;
    }
};
static_assert(sizeof(UnkRenderEntry_l071694) == 0x18, "UnkRenderEntry_l071694 layout");

using UnkRenderEntryVector_l071694 = base::global::CRntVector<UnkRenderEntry_l071694>;

UnkRenderEntryVector_l071694* fn_7101071694(UnkRenderEntryVector_l071694* self,
                                            const UnkRenderEntryVector_l071694* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkRenderEntry_l071694();
        self->count = 0;
        self->Reserve(other->capacity);
        for (const UnkRenderEntry_l071694 *it = other->data, *end = other->data + other->count; it != end; ++it)
        {
            // CRntVector::PushBackCopy, inlined: grow to the next multiple of the step.
            u32 n = self->count;
            u32 new_capacity = self->capacity;
            if (new_capacity <= n && self->step != 0)
                new_capacity = (n / self->step + 1) * self->step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            self->Reserve(new_capacity);
            new (&self->data[self->count]) UnkRenderEntry_l071694(*it);
            ++self->count;
        }
    }
    return self;
}
