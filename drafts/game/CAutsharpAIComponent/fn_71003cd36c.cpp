// Unit 572 (CAutsharpAIComponent): adds a target to the fixed list of 64 (u32 count at +0x8, 0x48-byte
// records from +0xc), then sorts the list by the comparator kept after it (+0x120c). Result: 1 added,
// 2 full, 3 added and now full. Names and layouts are placeholders.
#include "game/CAimComponent.h"
#include "types.h"

struct UnkEntry_71003cd6e0;
struct UnkCmp_71003cd6e0;
void fn_71003cd6e0(UnkEntry_71003cd6e0*, UnkEntry_71003cd6e0*, UnkCmp_71003cd6e0&);
s32 fn_71003cd4fc(const UnkCmp_71003cd6e0*, const UnkEntry_71003cd6e0&, const UnkEntry_71003cd6e0&);

// The comparator's state: a position and an axis.
struct UnkTargetOrder_l3cd36c {
    u32 words[6];

    UnkTargetOrder_l3cd36c(const UnkTargetOrder_l3cd36c& other) {
        for (u32 i = 0; i < 6; ++i)
            words[i] = other.words[i];
    }
};

#pragma pack(push, 4)
struct UnkTargetArray_l3cd36c {
    u32 count;
    UnkAimTarget items[0x40];
};

struct UnkTargetList_l3cd36c {
    void* vtable;
    UnkTargetArray_l3cd36c array;   // 0x8
    UnkTargetOrder_l3cd36c order;   // 0x120c
};
#pragma pack(pop)
static_assert(__builtin_offsetof(UnkTargetList_l3cd36c, order) == 0x120c, "layout");

struct UnkAddResult_l3cd36c {
    s32 code;
    UnkAddResult_l3cd36c(s32 c) : code(c) {}
    UnkAddResult_l3cd36c(const UnkAddResult_l3cd36c& other) : code(other.code) {}
};

static inline bool Before(const UnkTargetOrder_l3cd36c* order, const UnkAimTarget& a, const UnkAimTarget& b) {
    return fn_71003cd4fc(reinterpret_cast<const UnkCmp_71003cd6e0*>(order),
                         reinterpret_cast<const UnkEntry_71003cd6e0&>(a),
                         reinterpret_cast<const UnkEntry_71003cd6e0&>(b));
}

static inline UnkAddResult_l3cd36c Add(UnkTargetList_l3cd36c* self, const UnkAimTarget* target) {
    u32 count = self->array.count;
    s32 free = 0x40 - count;
    if (free <= 0) {
        UnkAddResult_l3cd36c result(2);
        u32 last = count - 1;
        const UnkAimTarget& back = self->array.items[last];
        // Faithful: the original assigns the last record to the slot at count - 1, which is itself.
        if (Before(&self->order, *target, back))
            self->array.items[self->array.count - 1] = back;
        return result;
    }
    new (&self->array.items[count]) UnkAimTarget(*target);
    self->array.count++;
    return UnkAddResult_l3cd36c(free == 1 ? 3 : 1);
}

UnkAddResult_l3cd36c fn_71003cd36c(UnkTargetList_l3cd36c* self, const UnkAimTarget* target) {
    UnkAddResult_l3cd36c result = Add(self, target);
    UnkAimTarget* begin = self->array.items;
    UnkAimTarget* end = begin + self->array.count;
    UnkTargetOrder_l3cd36c order = self->order;
    fn_71003cd6e0(reinterpret_cast<UnkEntry_71003cd6e0*>(begin), reinterpret_cast<UnkEntry_71003cd6e0*>(end),
                  reinterpret_cast<UnkCmp_71003cd6e0&>(order));
    return result;
}
