// The mesh's cell for a key (game/UnkNavMeshCell.h): the cells are a growable array of 0x70-byte
// records at +0x290 (count +0x298, capacity +0x29c, growth step +0x2a0, growing allowed +0x2a4). A key
// past the end grows the array (the reserve fn_7100279600 is called twice: to the next multiple of
// the step, then to the key) and constructs the new cells (fn_71002af630). Only the low 16 bits of the
// key are the index. Names and layouts are placeholders.
#include "game/UnkNavMeshCell.h"

// A cell's whole record (game/UnkNavMeshCell.h lays out its start).
struct UnkNavMeshCellRecord {
    UnkNavMeshCell cell;
    u8 unk_0x40[0x30];
};
static_assert(sizeof(UnkNavMeshCellRecord) == 0x70, "UnkNavMeshCellRecord layout");

struct UnkNavMeshCellArray {
    UnkNavMeshCellRecord* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool grow;
};

void fn_7100279600(UnkNavMeshCellArray* cells, u32 capacity);  // reserve
void fn_71002af630(UnkNavMeshCell* cell);                       // a cell's constructor

// NON_MATCHING(minor): same behaviour; the original counts the new cells down from (key - count) + 1,
// ours counts a negated counter up to zero (92%).
UnkNavMeshCell* fn_710027a33c(UnkNavMesh* mesh, u16 key) {
    auto* cells = reinterpret_cast<UnkNavMeshCellArray*>(reinterpret_cast<char*>(mesh) + 0x290);
    if (cells->grow) {
        u32 index = key;
        if (cells->count <= index) {
            u32 capacity = cells->capacity;
            if (capacity <= index && cells->step != 0)
                capacity = (index / cells->step + 1) * cells->step;
            fn_7100279600(cells, capacity);
            fn_7100279600(cells, index + 1);
            u32 count = cells->count;
            if (count <= index) {
                // Counted down from the number of new cells.
                u32 n = index - count + 1;
                u64 i = count;
                do {
                    fn_71002af630(&cells->data[i].cell);
                    ++i;
                } while (--n != 0);
            }
            cells->count = index + 1;
        }
    }
    return &cells->data[key].cell;
}
