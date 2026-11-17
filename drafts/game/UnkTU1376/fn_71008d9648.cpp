// UnkTU1376 (CDoorLifeComponent's unit): adds a navigation link to the door between the ends of two
// paths. Each end is the cell side at one end of its path, snapped round the corner towards the other
// end; the link is named after the door's entity and a label. Names and layouts are placeholders.
#include <cstdio>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::math::CVector2D;

struct UnkNode;
struct UnkSelf;
struct UnkWeakTarget;
struct UnkPoint2_l2b5aa8;
struct UnkLinkKeys_l2b5aa8;
struct UnkLinkRecord_l2b5aa8;

bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);
void fn_71002b5aa8(UnkLinkRecord_l2b5aa8* self, const u16* from_id, const u8* from_kind,
                   const UnkPoint2_l2b5aa8* from_point, const UnkVec3f* from_pos, u32 from_value, const u16* to_id,
                   const u8* to_kind, const UnkPoint2_l2b5aa8* to_point, const UnkVec3f* to_pos, u32 to_value,
                   const CStrId* name, bool flag_a, f32 a, f32 b, bool flag_b, bool flag_c, f32 c,
                   const CRntVector<u16>* entries, const UnkLinkKeys_l2b5aa8* keys_a,
                   const UnkLinkKeys_l2b5aa8* keys_b, const UnkLinkKeys_l2b5aa8* keys_c);
void fn_71002b6280(UnkSelf* self, UnkWeakTarget* other);
void fn_71002b563c(UnkNode* self, const CStrId* name, bool flag, const UnkLinkRecord_l2b5aa8* desc);

extern const CVector2D g_unk_710164b0f0 __attribute__((visibility("hidden")));
extern const UnkVec3f g_unk_710164b0f8 __attribute__((visibility("hidden")));

// The links follow their count directly (4-byte aligned).
typedef UnkNode* __attribute__((aligned(4))) UnkNodePtr;

struct UnkEntityName_l8d9648 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x44 - 0x18];
    u32 uses;  // 0x44
};

struct UnkDoorEntity_l8d9648 {
    u8 pad_0x0[0x30];
    UnkEntityName_l8d9648* name;  // 0x30
};

struct UnkDoorLinks_l8d9648 {
    u8 pad_0x0[0x30];
    UnkDoorEntity_l8d9648* entity;  // 0x30
    u8 pad_0x38[0x750 - 0x38];
    u32 count;            // 0x750
    UnkNodePtr links[1];  // 0x754
};

struct UnkPathStep_l8d9648 {
    u16 cell;
    u8 side;
};

struct UnkNavPath_l8d9648 {
    u8 pad_0x0[0x8];
    UnkPathStep_l8d9648* steps;  // 0x8
    u32 count;                   // 0x10
};

struct UnkLinkBuilder_l8d9648 {
    UnkDoorLinks_l8d9648* door;
    UnkNavMesh* mesh;
    const CRntVector<u16>* entries;
    const UnkLinkKeys_l2b5aa8* keys_a;
    const UnkLinkKeys_l2b5aa8* keys_b;
    const UnkLinkKeys_l2b5aa8* keys_c;
};

struct UnkEdgePoint_l8d9648 {
    f32 x, y;
    UnkEdgePoint_l8d9648() : x(0), y(0) {}
    UnkEdgePoint_l8d9648& operator=(const UnkEdgePoint_l8d9648& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

// A cell's edge record (0x38 bytes), as fn_71002afbe4 returns it.
struct UnkPathEdge_l8d9648 {
    u8 index;
    u8 field_0x1;
    u16 cell;  // 0x2
    u8 side;   // 0x4
    u8 field_0x5;
    u16 field_0x6;
    UnkEdgePoint_l8d9648 point;  // 0x8
    f32 field_0x10;
    u64 field_0x18[4];

    UnkPathEdge_l8d9648() : index(0xff), cell(0xffff), side(0xff), field_0x10(0), field_0x18() {}
};

struct UnkPathEnd_l8d9648 {
    UnkPathEdge_l8d9648 edge;
    CVector2D point;  // 0x38

    UnkPathEnd_l8d9648() : point{0, 0} {}
};

// The link description fn_71002b5aa8 builds (0x100 bytes); fn_71002b6094 destroys it.
struct alignas(8) UnkLinkDesc_l8d9648 {
    u8 pad_0x0[0x41];
    bool field_0x41;
    u8 pad_0x42[0x100 - 0x42];

    ~UnkLinkDesc_l8d9648() { fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(this)); }
};

// No side yet: both points at the origin and no length.
static inline void Clear(UnkNavMeshSideSegment* segment) {
    segment->field_0x10 = -1.0f;
    segment->start.x = 0;
    segment->start.y = 0;
    segment->end.x = 0;
    segment->end.y = 0;
}

void fn_71008d9648(UnkLinkBuilder_l8d9648* self, const UnkNavPath_l8d9648* from_path, bool from_forward,
                   const UnkNavPath_l8d9648* to_path, bool to_forward, const char* label) {
    UnkDoorLinks_l8d9648* door = self->door;

    // Each end of the link is its path's first or last side, whichever lies higher (forward) or lower.
    const UnkPathStep_l8d9648* from_first = &from_path->steps[0];
    UnkNavMeshCell* from_first_cell = fn_710027a33c(self->mesh, from_first->cell);
    UnkNavMeshSideSegment from_first_side;
    Clear(&from_first_side);
    fn_71002afea8(from_first_cell, &from_first->side, &from_first_side);
    const UnkPathStep_l8d9648* from_last = &from_path->steps[from_path->count - 1];
    UnkNavMeshCell* from_last_cell = fn_710027a33c(self->mesh, from_last->cell);
    UnkNavMeshSideSegment from_last_side;
    Clear(&from_last_side);
    fn_71002afea8(from_last_cell, &from_last->side, &from_last_side);
    UnkPathEnd_l8d9648 from;
    if (from_forward ? from_first_side.start.y > from_last_side.end.y
                     : from_first_side.start.y < from_last_side.end.y) {
        from.point = from_first_side.start;
        from.edge = *reinterpret_cast<const UnkPathEdge_l8d9648*>(fn_71002afbe4(from_first_cell, &from_first->side));
    } else {
        from.point = from_last_side.end;
        from.edge = *reinterpret_cast<const UnkPathEdge_l8d9648*>(fn_71002afbe4(from_last_cell, &from_last->side));
    }

    const UnkPathStep_l8d9648* to_first = &to_path->steps[0];
    UnkNavMeshCell* to_first_cell = fn_710027a33c(self->mesh, to_first->cell);
    UnkNavMeshSideSegment to_first_side;
    Clear(&to_first_side);
    fn_71002afea8(to_first_cell, &to_first->side, &to_first_side);
    const UnkPathStep_l8d9648* to_last = &to_path->steps[to_path->count - 1];
    UnkNavMeshCell* to_last_cell = fn_710027a33c(self->mesh, to_last->cell);
    UnkNavMeshSideSegment to_last_side;
    Clear(&to_last_side);
    fn_71002afea8(to_last_cell, &to_last->side, &to_last_side);
    UnkPathEnd_l8d9648 to;
    if (to_forward ? to_first_side.start.y > to_last_side.end.y : to_first_side.start.y < to_last_side.end.y) {
        to.point = to_first_side.start;
        to.edge = *reinterpret_cast<const UnkPathEdge_l8d9648*>(fn_71002afbe4(to_first_cell, &to_first->side));
    } else {
        to.point = to_last_side.end;
        to.edge = *reinterpret_cast<const UnkPathEdge_l8d9648*>(fn_71002afbe4(to_last_cell, &to_last->side));
    }

    // Each end moves round the corner of its cell towards the other.
    bool from_towards = from_forward ? from.point.x < to.point.x : from.point.x > to.point.x;
    u16 from_cell = 0xffff;
    u8 from_side = 0xff;
    if (fn_710027c460(self->mesh, from.edge.cell, from.edge.side, from_towards, &g_unk_710164b0f0, 0, &from_cell,
                      &from_side)) {
        from.edge.cell = from_cell;
        from.edge.side = from_side;
    }
    bool to_towards = to_forward ? from.point.x > to.point.x : from.point.x < to.point.x;
    u16 to_cell = 0xffff;
    u8 to_side = 0xff;
    if (fn_710027c460(self->mesh, to.edge.cell, to.edge.side, to_towards, &g_unk_710164b0f0, 0, &to_cell,
                      &to_side)) {
        to.edge.cell = to_cell;
        to.edge.side = to_side;
    }
    UnkLinkDesc_l8d9648 desc;
    fn_71002b5aa8(reinterpret_cast<UnkLinkRecord_l2b5aa8*>(&desc), &from.edge.cell, &from.edge.side,
                  reinterpret_cast<const UnkPoint2_l2b5aa8*>(&from.point), &g_unk_710164b0f8, 0, &to.edge.cell,
                  &to.edge.side, reinterpret_cast<const UnkPoint2_l2b5aa8*>(&to.point), &g_unk_710164b0f8, 0,
                  &g_unk_7101d470e0, false, 0, 0, false, false, 0, self->entries, self->keys_a, self->keys_b,
                  self->keys_c);
    fn_71002b6280(reinterpret_cast<UnkSelf*>(&desc), reinterpret_cast<UnkWeakTarget*>(door->entity));
    desc.field_0x41 = true;

    door->entity->name->uses++;
    const char* text = door->entity->name->text;
    char name[0x100];
    sprintf(name, "%s [%s]", text ? text : g_unk_710162e2ed, label);
    UnkNode* link = static_cast<UnkNode*>(fn_71000839a0(0x110, 4));
    CStrId id(name, true);
    fn_71002b563c(link, &id, true, reinterpret_cast<const UnkLinkRecord_l2b5aa8*>(&desc));
    door->links[door->count++] = link;
}
