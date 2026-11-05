// Unit 2565 (tools/tu_map.py): the clear slot (CCollectionType +0x110) of an array with a second pointer array
// alongside (UnkSecondPtrArray, include/base/UnkFreedVectorPair.h) of 0x248-byte records that start with a
// string with inline storage: each is destroyed in its two steps (fn_710007ed40, then the string's destructor
// fn_710007ecd4), the storage kept. Names and layouts are placeholders.
#include "base/UnkFreedVectorPair.h"
#include "base/global/CRntString.h"
#include "types.h"

struct UnkInlineStringRecord_A6_b76 {
    // The string's storage (the string is destroyed by hand below).
    alignas(8) u8 str[0x30];
    u8 unk_0x30[0x218];

    ~UnkInlineStringRecord_A6_b76() {
        fn_710007ed40(reinterpret_cast<base::global::CRntString*>(str));
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(str));
    }
};
static_assert(sizeof(UnkInlineStringRecord_A6_b76) == 0x248, "layout");

void fn_7100e7b5b4(UnkSecondPtrArray<UnkInlineStringRecord_A6_b76>* self) {
    if (self->data)
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkInlineStringRecord_A6_b76();
    self->count = 0;
}
