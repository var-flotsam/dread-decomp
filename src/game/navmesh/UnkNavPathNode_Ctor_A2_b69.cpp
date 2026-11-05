// The navigation search node's unit (unit 358, src/game/navmesh/UnkNavPathNode.cpp): the constructor that
// takes the cell, side, kind and position (fn_71002871cc and fn_7100287298 in one).
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkObject_0x100.h"
#include "types.h"

void fn_71002b57bc(UnkObject_0x100* self);  // the payload's constructor

// Same layouts as src/game/navmesh/UnkNavPathNode.cpp.
struct UnkNavPathPoint {
    f32 x;
    f32 y;
};

struct UnkNavPathNode {
    u8 data[0x100];
    u16 cell;
    u8 side;
    u32 kind;
    u32 field_0x108;
    UnkNavPathPoint pos;
    f32 costs[3];
    UnkNavPathNode* parent;
};
static_assert(sizeof(UnkNavPathNode) == 0x128, "UnkNavPathNode layout");

u32 fn_71002b009c(UnkNavMeshCell* cell);

void fn_710028720c(UnkNavPathNode* self, UnkNavMesh* mesh, u16 cell, u8 side, u32 kind, const UnkNavPathPoint* pos) {
    fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(self->data));
    self->costs[1] = 0.0f;
    self->costs[2] = 0.0f;
    self->parent = nullptr;
    self->cell = cell;
    self->side = side;
    self->kind = kind;
    self->field_0x108 = 0;
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->costs[0] = 0.0f;
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    if (mesh && cell != 0xffff) {
        UnkNavMeshCell* c = fn_710027a328(mesh, cell);
        u32 w = fn_71002b009c(c);
        asm("" : "+r"(self));
        self->field_0x108 = w;
    }
}
