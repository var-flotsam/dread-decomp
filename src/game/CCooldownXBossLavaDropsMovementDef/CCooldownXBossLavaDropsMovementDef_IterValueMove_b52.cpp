// CCooldownXBossLavaDropsMovementDef's unit (tools/tu_map.py): two identical move helpers
// (CType+0x38) of the iterator value over a reflected array of 0x18-byte polymorphic elements, as
// base::refl_IterValueMove: the (index, element) pair a value owns or refers to is copied in, the
// element's vtable (unk_7101a3e068) set. Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "types.h"

extern char g_unk_7101a3e068[];

// Three words copied as one 12-byte block (a vector, perhaps).
struct UnkLavaDropsData_b52 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkLavaDropsElem_b52 {
    char* vtable;
    UnkLavaDropsData_b52 data;  // 0x8

    UnkLavaDropsElem_b52(UnkLavaDropsElem_b52&& other) : vtable(g_unk_7101a3e068 + 0x10), data(other.data) {}
};

void fn_71004b1fd4(void* dst, const void* src) {
    base::refl_IterValueMove<UnkLavaDropsElem_b52>(dst, src);
}

void fn_71004b2288(void* dst, const void* src) {
    base::refl_IterValueMove<UnkLavaDropsElem_b52>(dst, src);
}
