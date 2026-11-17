// CHeatableShieldComponent unit (unit 1322): releases the shield's four sets of lists, last first; each set
// clears the slots its first list points at, empties its lists and frees their arrays.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

// Three lists; the first holds the addresses of slots that refer back to this set.
struct UnkHeatableListSet_l86ae60 {
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
static_assert(sizeof(UnkHeatableListSet_l86ae60) == 0x80, "UnkHeatableListSet_l86ae60 layout");

// The block src/game/CHeatableShieldComponent/CHeatableShieldComponent_Entries_ovb0080.cpp passes.
struct UnkHeatableExtra_b0080 {
    UnkHeatableListSet_l86ae60 sets[4];
};

void fn_710086ae60(UnkHeatableExtra_b0080* self) {
    self->sets[3].Release();
    self->sets[2].Release();
    self->sets[1].Release();
    self->sets[0].Release();
}
