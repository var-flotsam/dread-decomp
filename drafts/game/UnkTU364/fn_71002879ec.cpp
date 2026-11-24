// Unit 364 (tools/tu_map.py): sets up a per-cell table of side flags for a navigation mesh: one
// byte array per cell, a cleared byte for each of its sides, then two groups of three empty
// arrays. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/navmesh/CNavMesh.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CRntVector;

UnkOwnedList* GetOrGrowOwnedListAt(base::global::CRntVector<UnkOwnedList>* self, u32 index);

struct UnkArrayGroup_l2879ec {
    CRntVector<void*> first;
    CRntVector<void*> second;
    CRntVector<void*> third;
    bool flag;

    UnkArrayGroup_l2879ec() : first(0xf), second(0xf), third(0xf), flag(false) {}
};

struct UnkSideTable_l2879ec {
    CRntVector<CRntVector<u8>> cells;  // 0x0, one flag per side of each cell
    UnkArrayGroup_l2879ec groups[2];   // 0x28

    // The flags of one cell, growing the table to reach it.
    CRntVector<u8>* Cell(u32 index) {
        return reinterpret_cast<CRntVector<u8>*>(
            GetOrGrowOwnedListAt(reinterpret_cast<CRntVector<UnkOwnedList>*>(&cells), index));
    }
};
static_assert(sizeof(UnkSideTable_l2879ec) == 0x128, "layout");

// Clears the flag at `index`, first growing the array to reach it when that is allowed: to the
// next multiple of the step, then to exactly index + 1 flags, the new ones cleared.
static void ClearFlag(CRntVector<u8>* flags, u32 index) {
    if (flags->field_0x14 && flags->count <= index) {
        u32 cap = flags->capacity;
        if (cap <= index && flags->step != 0) {
            u32 grown = (index / flags->step + 1) * flags->step;
            if (cap < grown) {
                flags->Reserve(grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            flags->Reserve(n);
        __builtin_memset(&flags->data[flags->count], 0, n - flags->count);
        flags->count = n;
    }
    flags->data[index] = 0;
}

void fn_71002879ec(void* object, UnkNavMesh* mesh) {
    UnkSideTable_l2879ec* self = static_cast<UnkSideTable_l2879ec*>(object);
    new (&self->cells) CRntVector<CRntVector<u8>>(0x57);
    new (&self->groups[0]) UnkArrayGroup_l2879ec();
    new (&self->groups[1]) UnkArrayGroup_l2879ec();
    self->cells.Reserve(fn_710027a320(mesh));
    for (u16 i = 0; i < fn_710027a320(mesh); ++i) {
        *self->Cell(i) = CRntVector<u8>(0x57);
        self->Cell(i)->Reserve(fn_710027a328(mesh, i)->field_0x10);
        for (u32 j = 0; j < fn_710027a328(mesh, i)->field_0x10; ++j)
            ClearFlag(self->Cell(i), j);
    }
}
