// The target list's unit (translation unit 1038 of data/tu_map.csv, see UnkScourgeTargetList.cpp):
// hands the weak handle of a new target to the two slots at +0x260 and +0x2d8.
#include "game/UnkTU2523.h"
#include "game/UnkTU852.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// Takes a counted reference to a weak handle (the caller drops its own).
struct UnkHandleList;

static inline UnkRefPtr MakeRef(UnkWeakHandle* handle) {
    UnkRefPtr ref;
    ref.field_0x0 = nullptr;
    if (handle) {
        ++handle->ref_count;
        ref.field_0x0 = handle;
    }
    return ref;
}

void fn_71006a21dc(char* self, UnkWeakTarget* target) {
    if (!target)
        return;
    fn_710059cecc(reinterpret_cast<UnkHandleList*>(self + 0x260), MakeRef(fn_7100d78378(target)));
    fn_710059cecc(reinterpret_cast<UnkHandleList*>(self + 0x2d8), MakeRef(fn_7100d78378(target)));
}
