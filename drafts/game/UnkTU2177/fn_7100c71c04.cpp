// Unit 2177: copies the entry at an index of one fixed list, then the entries after it while each lies
// within a tolerance of the first (wider when either belongs to the owner's current target), into
// another list. Names and layouts are placeholders.
#include <new>

#include "types.h"

// Packed to 4 bytes: the list holds the entries straight after its 4-byte count.
#pragma pack(push, 4)
struct UnkNearEntry_lc71c04 {
    f32 x;              // 0x0
    f32 y;              // 0x4
    u32 field_0x8;      // 0x8
    f32 layer;          // 0xc
    void* owner;        // 0x10
    u32 field_0x18;     // 0x18
    u32 field_0x1c;     // 0x1c
    u32 field_0x20;     // 0x20
    u16 field_0x24;     // 0x24

    UnkNearEntry_lc71c04()
        : x(0.0f), y(0.0f), field_0x8(0), layer(0.0f), owner(nullptr), field_0x18(0), field_0x1c(0),
          field_0x20(0), field_0x24(0) {}
    UnkNearEntry_lc71c04(const UnkNearEntry_lc71c04& o) { *this = o; }
    // Member by member.
    UnkNearEntry_lc71c04& operator=(const UnkNearEntry_lc71c04& o) {
        x = o.x;
        y = o.y;
        field_0x8 = o.field_0x8;
        layer = o.layer;
        owner = o.owner;
        field_0x18 = o.field_0x18;
        field_0x1c = o.field_0x1c;
        field_0x20 = o.field_0x20;
        field_0x24 = o.field_0x24;
        return *this;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkNearEntry_lc71c04) == 0x28, "UnkNearEntry_lc71c04 size");

// A list of entries held in place, growing (zero-filled) when an index past its end is asked for.
struct UnkNearList_lc71c04 {
    u32 count;
    UnkNearEntry_lc71c04 items[1];  // capacity unknown

    UnkNearEntry_lc71c04& At(u32 index) {
        if (index + 1 > count) {
            for (u32 i = count; i < index + 1; i++)
                new (&items[i]) UnkNearEntry_lc71c04();
            count = index + 1;
        }
        return items[index];
    }

    void PushBack(const UnkNearEntry_lc71c04& entry) {
        items[count] = entry;
        ++count;
    }
};

struct UnkNearOwner_lc71c04 {
    u8 pad_0x0[0x5f40];
    void* target;  // 0x5f40
};

void fn_7100c71c04(UnkNearOwner_lc71c04* self, UnkNearList_lc71c04* src, u32 index, UnkNearList_lc71c04* dst) {
    UnkNearEntry_lc71c04 first = src->At(index);
    dst->PushBack(first);
    u32 count = src->count;
    for (u32 i = index + 1; i < count; i++) {
        UnkNearEntry_lc71c04 entry = src->At(i);
        f32 tolerance = (first.owner == self->target || entry.owner == self->target) ? 1.0f : 0.1f;
        bool near = first.x <= entry.x + tolerance && first.x >= entry.x - tolerance &&
                    entry.layer == first.layer && first.y <= entry.y + tolerance &&
                    first.y >= entry.y - tolerance;
        if (!near)
            return;
        dst->PushBack(entry);
    }
}
