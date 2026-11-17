// Unit 263 (minimap rendering): sorts a list of render items into four lists by their type and
// kind, then sorts the last list. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

struct UnkRenderItem_26a4 {
    u32 type;
    u8 pad_0x4[0x14];
    u32 kind;  // 0x18
};

struct UnkRenderLists_26a4 {
    u8 pad_0x0[0x8];
    CRntVector<UnkRenderItem_26a4*> others;  // 0x8
    CRntVector<UnkRenderItem_26a4*> kind2;   // 0x30
    CRntVector<UnkRenderItem_26a4*> kind3;   // 0x58
    CRntVector<UnkRenderItem_26a4*> sorted;  // 0x80
};

// std::sort's helper over a range of item pointers (src/game_s7/fn_71001e2e30.cpp).
void fn_71001e2e30(UnkRenderItem_26a4** first, UnkRenderItem_26a4** last);

// CRntVector::PushBack, growing through Reserve; the element is copied bitwise, so the count
// is reloaded after the store.
template <class T>
inline void PushBack(CRntVector<T>& v, const T& value) {
    u32 n = v.count;
    u32 new_capacity = v.capacity;
    if (v.capacity <= n && v.step != 0)
        new_capacity = (n / v.step + 1) * v.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    v.Reserve(new_capacity);
    __builtin_memcpy(&v.data[v.count], &value, sizeof(T));
    ++v.count;
}

void fn_71001e26a4(UnkRenderLists_26a4* self, UnkRenderItem_26a4** items, u32 count) {
    self->others.count = 0;
    self->kind2.count = 0;
    self->kind3.count = 0;
    self->sorted.count = 0;
    for (u32 i = 0; i < count; ++i) {
        UnkRenderItem_26a4* item = items[i];
        if (!item)
            continue;
        switch (item->type) {
        case 2:
        case 0x2000:
            switch (item->kind) {
            case 2:
                PushBack(self->kind2, item);
                break;
            case 3:
                PushBack(self->kind3, item);
                break;
            case 4:
                PushBack(self->sorted, item);
                break;
            default:
                PushBack(self->others, item);
                break;
            }
            break;
        case 0x80:
            PushBack(self->sorted, item);
            break;
        }
    }
    fn_71001e2e30(self->sorted.data, self->sorted.data + self->sorted.count);
}
