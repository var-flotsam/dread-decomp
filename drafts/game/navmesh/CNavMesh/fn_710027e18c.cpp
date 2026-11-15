// navmesh::CNavMesh: removes a link from the per-cell link list at +0x3b0 (the inverse of the add
// in CNavMesh_Misc_ovb0018.cpp): the last entry takes its place. Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkClone_fn_71002797bc;
using UnkNavLinkSlotE18c = UnkCloneElem<UnkClone_fn_71002797bc, 8>;
extern template UnkNavLinkSlotE18c& base::global::CRntVector<UnkNavLinkSlotE18c>::GrowAt(u32);

struct UnkNavLinkE18c {
    u8 pad_0x0[0xa];
    u16 cell;
};

struct UnkNavLinkListE18c {
    UnkNavLinkE18c** data;
    u32 count;
};

struct UnkNavMeshLinksE18c {
    u8 pad_0x0[0x3b0];
    base::global::CRntVector<UnkNavLinkSlotE18c> links;  // +0x3b0, per cell
};

void fn_710027e18c(UnkNavMeshLinksE18c* self, UnkNavLinkE18c* link) {
    u16 cell = link->cell;
    if (cell == 0xffff)
        return;
    UnkNavLinkListE18c* list = *reinterpret_cast<UnkNavLinkListE18c**>(self->links.GrowAt(cell).data);
    if (!list)
        return;
    u64 found = 0;
    u64 end = 0;
    u32 count = list->count;
    for (u64 i = 0; i != count; ++i) {
        if (list->data[i] == link) {
            found = i;
            end = count;
            break;
        }
    }
    if (found == end)
        return;
    if (count <= (u32)found)
        return;
    u32 last = count - 1;
    if (last > (u32)found)
        list->data[found] = list->data[last];
    list->count = last;
}
