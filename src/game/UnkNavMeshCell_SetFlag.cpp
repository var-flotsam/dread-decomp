// A navigation-mesh cell's unit (tools/tu_map.py unit 367, beside src/game/UnkNavMeshCell.cpp): sets
// the cell's flag at +0x60 and, when it changed, tells the owner at +0x58 (fn_710027da54) with the
// cell's id and the new value.
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU367.h"
#include "game/navmesh/CNavMesh.h"
#include "types.h"

void fn_71002af93c(UnkNavMeshCell* self, bool flag) {
    if (self->flag == flag) {
        return;
    }
    self->flag = flag;
    if (self->owner != nullptr) {
        fn_710027da54(self->owner, self->Id(), flag);
    }
}
