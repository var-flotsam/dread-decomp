#pragma once

// CNavMesh: functions and globals of this translation unit that other files use (tools/tu_map.py unit 355,
// 0x7100274548-0x71002810b0). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshCell.h"
#include "types.h"

struct UnkNavMesh;
struct UnkDirectionFilter;
struct UnkNavMeshHits;
struct UnkNavMeshSideSegment;
template <class R, class... A>
struct UnkNavMeshCallback;
struct UnkNavLoadCell;
struct UnkOwner_710027a41c;
bool fn_710027a5f0(UnkOwner_710027a41c* self, const base::math::CVector2D* point, u16 index);
void fn_7100279600(base::global::CRntVector<UnkNavLoadCell>* self, u32 capacity);  // The cell array's reserve.
void fn_710027ebe4(base::global::CRntVector<UnkNavMeshEdge>* self, const base::global::CRntVector<UnkNavMeshEdge>* other);
u32 fn_710027a320(UnkNavMesh* mesh);
// Sends a cell's 16-bit id to one of the mesh's two deferred callback sets (+0x90 when `first`, else +0x110);
// a cell calls it when its flag changes (src/game/UnkNavMeshCell_SetFlag.cpp).
void fn_710027da54(UnkNavMesh_l27da54* self, const u16* value, bool first);
void fn_710027e258(void* self, const void* other);
extern char g_unk_7101a20e58[];

// The nearest side to `point` that passes the filters, added to `out` (src/game/navmesh/CNavMesh.cpp).
bool fn_710027b780(UnkNavMesh* mesh, const base::math::CVector2D* point, u32 flags, u64 filter_arg, u32 filter_arg2,
                   u32 filter_arg3, UnkDirectionFilter* direction, UnkNavMeshHits* out,
                   const UnkNavMeshCallback<bool, u16, u8, void*, void*, u64, u32, u32>& filter,
                   const UnkNavMeshCallback<s32, UnkNavMeshSideSegment*>& verdict);
