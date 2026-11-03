// A navigation-mesh cell's unit (tools/tu_map.py unit 367, beside src/game/UnkNavMeshCell.cpp): sets
// the cell's flag at +0x60 and, when it changed, tells the owner at +0x58 (fn_710027da54) with the
// cell and the new value. The cell is seen through a placeholder layout of its own (the shared one in
// include/game/UnkNavMeshCell.h stops at +0x3c).
#include "types.h"

struct UnkNavMeshCellFlag367 {
    u8 pad_0x0[0x58];
    void* owner;  // 0x58
    bool flag;    // 0x60
};

void fn_710027da54(void* owner, UnkNavMeshCellFlag367* cell, bool flag);

void fn_71002af93c(UnkNavMeshCellFlag367* self, bool flag) {
    if (self->flag == flag) {
        return;
    }
    self->flag = flag;
    if (self->owner != nullptr) {
        fn_710027da54(self->owner, self, flag);
    }
}
