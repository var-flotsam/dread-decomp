// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): releases four sets of lists,
// last first; each set clears the slots its first list points at, empties its lists and frees their arrays.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

// Three lists; the first holds the addresses of slots that refer back to this set.
struct UnkListSet_l8cf3ac {
    CRntVector<void**> slots;
    CRntVector<void*> field_0x28;
    CRntVector<void*> field_0x50;
    u8 pad_0x78[0x8];

    void Release() {
        void*** end = slots.data + slots.count;
        for (void*** it = slots.data; it != end; ++it)
            **it = nullptr;
        slots.Clear();
        field_0x28.Clear();
        field_0x50.Clear();
        field_0x50.Free();
        field_0x28.Free();
        slots.Free();
    }
};
static_assert(sizeof(UnkListSet_l8cf3ac) == 0x80, "UnkListSet_l8cf3ac layout");

struct UnkListSets_l8cf3ac {
    UnkListSet_l8cf3ac sets[4];
};

void fn_71008cf3ac(UnkListSets_l8cf3ac* self) {
    self->sets[3].Release();
    self->sets[2].Release();
    self->sets[1].Release();
    self->sets[0].Release();
}
