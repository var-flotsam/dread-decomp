#pragma once

// A cell of the navigation mesh (the objects fn_710027a328 of navmesh::CNavMesh hands out). Its
// functions are tools/tu_map.py unit 367, fn_71002af630 to fn_71002b106c, the unit after
// navmesh::SNavMeshPath's. A cell is a polygon: the vertex ids at +0x8 (count at +0x10) and a table
// of 0x38-byte edge records at +0x30 (count at +0x38), each tagged with the index of the side it is
// on. The functions here look an edge up by that index; fn_71002afc84, fn_71002afcd8 (the
// crossing test fn_7100ebfa7c uses) and fn_71002afe60 (the vertex pair of a side) belong to the
// same unit. Only the members the hand-written files touch are spelled out; names and layouts are
// placeholders inferred from the code.

#include "types.h"

struct UnkNavMeshEdge {
    u8 index;  // the side of the cell this edge is on (the key the lookups compare)
    u8 pad_0x1;
    u16 field_0x2;  // the cell on the other side (fn_7100ebfa7c queries it)
    u8 pad_0x4[0x8];
    f32 field_0xc;
    f32 field_0x10;
    u8 pad_0x14[0x24];
};
static_assert(sizeof(UnkNavMeshEdge) == 0x38, "UnkNavMeshEdge layout");

struct UnkNavMeshCell {
    u8 pad_0x0[0x8];
    u16* vertices;  // +0x8, the vertex ids
    u8 field_0x10;  // the number of sides (fn_71002afe60 reads the whole word)
    u8 pad_0x11[0x1f];
    UnkNavMeshEdge* edges;  // +0x30
    u32 edge_count;         // +0x38
};
static_assert(__builtin_offsetof(UnkNavMeshCell, edges) == 0x30, "UnkNavMeshCell placeholder layout");
static_assert(__builtin_offsetof(UnkNavMeshCell, edge_count) == 0x38, "UnkNavMeshCell placeholder layout");

// The record the lookups return when a cell has no edge for the side (the same object for every
// cell, so callers compare the pointer).
extern UnkNavMeshEdge g_unk_71726a4ae8;

// The cell's edge on side `*index`, or g_unk_71726a4ae8. Defined in src/game/UnkNavMeshCell.cpp,
// with the read-only twin fn_71002afc34 (the same code).
UnkNavMeshEdge* fn_71002afbe4(UnkNavMeshCell* cell, const u8* index);
const UnkNavMeshEdge* fn_71002afc34(const UnkNavMeshCell* cell, const u8* index);

// The vertex ids at either end of a side (0xffff, 0xffff when there is no such side). Returned in
// memory, as the callers' copy constructor makes it.
struct UnkNavMeshSidePair {
    union {
        u32 raw;  // both ids at once (all ones when there is no side)
        struct {
            u16 a;
            u16 b;
        };
    };
    UnkNavMeshSidePair() {}
    UnkNavMeshSidePair(const UnkNavMeshSidePair&);
};

// The vertex pair of side `*index` (src/game/UnkNavMeshCell.cpp).
UnkNavMeshSidePair fn_71002afe60(UnkNavMeshCell* cell, u8* index);

// A test on a cell (unit 367 too); callers skip cells it rejects.
bool fn_71002b005c(UnkNavMeshCell* cell);

// The mesh's cell for a key (navmesh::CNavMesh, next to fn_710027a328), growing its tables to reach it
// (src/game/navmesh/CNavMesh/fn_710027a33c.cpp). The key is 16 bits: the callee widens it itself.
struct UnkNavMesh;
UnkNavMeshCell* fn_710027a33c(UnkNavMesh* mesh, u16 key);
// The mesh's cell for a key, without growing anything (navmesh::CNavMesh; generated, src/lifted/).
UnkNavMeshCell* fn_710027a328(UnkNavMesh* mesh, u16 cell);
