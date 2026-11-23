// The navigation search node's unit (unit 358 of tools/tu_map.py, fn_71002871cc to fn_710028739c):
// src/game/navmesh/UnkNavMeshQueryState.cpp lays the node out and calls these out of line. Holds the
// constructor (fn_71002871cc) and the setter of its place (fn_7100287298). Names and layouts are
// placeholders.
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkNavPathNode.h"
#include "game/UnkNavPathPoint.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU367.h"
#include "game/UnkTU376.h"
#include "types.h"

static_assert(sizeof(UnkNavPathNode) == 0x128, "UnkNavPathNode layout");

// An empty node: no cell (0xffff) and no side (0xff), everything after zero.
void fn_71002871cc(UnkNavPathNode* self) {
    fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(self->data));
    self->cell = 0xffff;
    self->side = 0xff;
    self->kind = 0;
    self->field_0x108 = 0;
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->costs[0] = 0.0f;
    self->costs[1] = 0.0f;
    self->costs[2] = 0.0f;
    self->parent = nullptr;
}

// Sets the cell, side, kind and position; with a mesh and a cell, also +0x108 from the cell.
void fn_7100287298(UnkNavPathNode* self, UnkNavMesh* mesh, u16 cell, u8 side, u32 kind, const UnkNavPathPoint* pos) {
    self->cell = cell;
    self->side = side;
    self->kind = kind;
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    if (mesh && cell != 0xffff)
        self->field_0x108 = fn_71002b009c(fn_710027a328(mesh, cell));
}
