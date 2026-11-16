// Unit 356: initialises the navigation-mesh query state: no mesh, empty node lists, a search in
// status 1 with its query at +0x5dc20 set up and no current cell or point. Names and layouts are
// placeholders.
#include "game/UnkTU356.h"
#include "game/UnkTU357.h"
#include "types.h"

// The node pool: free node pointers, then the node records.
struct UnkNavNodePool_l2810b0 {
    u32 free_count;
    u8 pad_0x4[0x2584 - 0x4];
    u32 node_count;  // +0x2584
    u8 pad_0x2588[0x5dc18 - 0x7098];

    void Init() {
        free_count = 0;
        node_count = 0;
    }
};

struct UnkNavSearch_l2810b0 {
    u32 status;
    u8 pad_0x4[4];
    u8 query[0x1b8];  // +0x8
    void* field_0x1c0;
    u16 cell;  // +0x1c8
    u8 side;   // +0x1ca
    u8 pad_0x1cb;
    f32 point[2];  // +0x1cc
    f32 field_0x1d4;

    void ClearCurrent() {
        field_0x1c0 = nullptr;
        cell = 0xffff;
        side = 0xff;
        point[0] = 0.0f;
        point[1] = 0.0f;
        field_0x1d4 = 0.0f;
    }
};

struct UnkNavQueryState_l2810b0 {
    void* mesh;
    u32 open_count;  // +0x8
    u8 pad_0xc[0x258c - 0xc];
    u32 closed_count;  // +0x258c
    u8 pad_0x2590[0x4b10 - 0x2590];
    UnkNavNodePool_l2810b0 pool;    // +0x4b10
    UnkNavSearch_l2810b0 search;    // +0x5dc18
};
static_assert(__builtin_offsetof(UnkNavQueryState_l2810b0, closed_count) == 0x258c, "layout");
static_assert(__builtin_offsetof(UnkNavQueryState_l2810b0, pool) == 0x4b10, "layout");
static_assert(__builtin_offsetof(UnkNavQueryState_l2810b0, search) == 0x5dc18, "layout");
static_assert(sizeof(UnkNavQueryState_l2810b0) == 0x5ddf0, "layout");

void fn_71002810b0(UnkNavMeshState_0x5ddf0* state) {
    UnkNavQueryState_l2810b0* self = reinterpret_cast<UnkNavQueryState_l2810b0*>(state);
    self->mesh = nullptr;
    self->open_count = 0;
    self->closed_count = 0;
    self->pool.Init();
    self->search.status = 1;
    fn_710028659c(reinterpret_cast<UnkNavQuery*>(self->search.query));
    self->search.ClearCurrent();
}
