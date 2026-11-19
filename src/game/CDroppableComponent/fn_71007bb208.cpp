// CDroppableComponent unit (tools/tu_map.py, unit 1216): appends a copy of a 0x48-byte record (copied by
// fn_7100331c7c) to a fixed list of 32 (u32 count at +0x8, records from +0xc). Result code: 1 added, 2 full,
// 3 added and now full. Names and layouts are placeholders.
#include "game/CAbilityShinespark.h"
#include "types.h"

#include <new>

#pragma pack(push, 4)
struct UnkDropEntry_l7bb208 {
    u64 words[9];
    UnkDropEntry_l7bb208(const UnkDropEntry_l7bb208& other) { fn_7100331c7c(this, &other); }
};

struct UnkAddResult_l7bb208 {
    s32 code;
    UnkAddResult_l7bb208(s32 c) : code(c) {}
    UnkAddResult_l7bb208(const UnkAddResult_l7bb208& other) : code(other.code) {}
};

struct UnkDropArray_l7bb208 {
    u32 count;
    UnkDropEntry_l7bb208 items[32];

    UnkAddResult_l7bb208 Add(const UnkDropEntry_l7bb208& entry) {
        s32 free = (s32)(32 - count);
        if (free > 0) {
            new (&items[count]) UnkDropEntry_l7bb208(entry);
            ++count;
            return UnkAddResult_l7bb208(free == 1 ? 3 : 1);
        }
        return UnkAddResult_l7bb208(2);
    }
};

struct UnkDropList_l7bb208 {
    void* vtable;
    UnkDropArray_l7bb208 array;
};
#pragma pack(pop)
static_assert(sizeof(UnkDropEntry_l7bb208) == 0x48, "entry");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkAddResult_l7bb208 fn_71007bb208(UnkDropList_l7bb208* self, const UnkDropEntry_l7bb208& entry) {
    return self->array.Add(entry);
}
