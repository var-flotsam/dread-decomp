// CCooldownXBossFireBallDef's unit (tools/tu_map.py): two identical move helpers
// (CType+0x38) of the iterator value over a reflected array of 0x18-byte polymorphic elements, as
// base::refl_IterValueMove: the (index, element) pair a value owns or refers to is copied in, the
// element's vtable (unk_7101a3e2b0) set. Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "game/CCooldownXBossFireBallDef.h"
#include "types.h"

// Two words and a flag, copied as one 9-byte block (packed: the copy skips the tail padding).
struct __attribute__((packed)) UnkFireBallData_b52 {
    f32 field_0x0;
    f32 field_0x4;
    bool field_0x8;
};

struct UnkFireBallElem_b52 {
    char* vtable;
    UnkFireBallData_b52 data;  // 0x8

    UnkFireBallElem_b52(UnkFireBallElem_b52&& other) : vtable(g_unk_7101a3e2b0 + 0x10), data(other.data) {}
};

void fn_71004bfbe4(void* dst, const void* src) {
    base::refl_IterValueMove<UnkFireBallElem_b52>(dst, src);
}

void fn_71004bfe98(void* dst, const void* src) {
    base::refl_IterValueMove<UnkFireBallElem_b52>(dst, src);
}
