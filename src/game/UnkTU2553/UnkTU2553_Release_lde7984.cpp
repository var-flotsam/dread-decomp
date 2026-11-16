// Unit 2553 (CGameManager::CTunableGameManager): releases three sets of lists, last first; each set clears
// the slots its first list points at, empties its lists and frees their arrays. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

// Three lists; the first holds the addresses of slots that refer back to this set.
struct UnkListSet_lde7984 {
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
static_assert(sizeof(UnkListSet_lde7984) == 0x80, "UnkListSet_lde7984 layout");

struct UnkListSets_lde7984 {
    UnkListSet_lde7984 field_0x0;
    UnkListSet_lde7984 field_0x80;
    UnkListSet_lde7984 field_0x100;
};

void fn_7100de7984(UnkListSets_lde7984* self) {
    self->field_0x100.Release();
    self->field_0x80.Release();
    self->field_0x0.Release();
}
