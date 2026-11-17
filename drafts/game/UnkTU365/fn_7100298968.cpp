// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator): links the shared edges of a list of polygons:
// clears every polygon's edge links, then for each pair whose bounds (grown by 50) overlap, joins each edge
// with the first edge of the other running the opposite way within a unit. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU108.h"
#include "types.h"

using base::math::CVector2D;

struct UnkNavVertex_l298968 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkBox2D_l298968 {
    CVector2D min;
    CVector2D max;

    void Extend(const CVector2D& p) {
        min.x = min.x < p.x ? min.x : p.x;
        min.y = min.y < p.y ? min.y : p.y;
        max.x = max.x > p.x ? max.x : p.x;
        max.y = max.y > p.y ? max.y : p.y;
    }
    bool Overlaps(const UnkBox2D_l298968& other) const {
        return other.max.x >= min.x && other.min.x <= max.x && other.max.y >= min.y && other.min.y <= max.y;
    }
};

struct UnkNavPolygon_l298968;

// The polygon an edge is joined to, and that polygon's edge.
struct UnkEdgeLink_l298968 {
    UnkNavPolygon_l298968* polygon;
    u32 edge;
};

// A singly linked list of edge links, one per edge.
struct UnkEdgeLinks_l298968 {
    struct Node {
        UnkEdgeLink_l298968 value;
        Node* next;
    };
    u32 count;
    Node* head;

    UnkEdgeLink_l298968& At(u64 index) {
        Node* node = head;
        for (u32 k = 0; k != index; ++k)
            node = node->next;
        return node->value;
    }
    void Set(u64 index, const UnkEdgeLink_l298968& link) {
        if (index < count)
            At(index) = link;
    }
};

struct UnkNavPolygon_l298968 {
    UnkNavVertex_l298968* vertices;
    u32 count;
    u8 pad_0xc[0x20];
    u8 closed;        // 0x2c
    u8 bounds_dirty;  // 0x2d
    u8 pad_0x2e[2];
    UnkBox2D_l298968 bounds;  // 0x30
    u8 pad_0x40[0x10];
    UnkEdgeLinks_l298968 links;  // 0x50

    UnkSegment2D Edge(u64 i) const {
        return UnkSegment2D(vertices[i].pos, count - 1 == i ? vertices[0].pos : vertices[i + 1].pos);
    }
    const UnkBox2D_l298968& Bounds() {
        if (bounds_dirty)
            fn_7100119b88(reinterpret_cast<UnkPolygon2D*>(this));
        return bounds;
    }
};
static_assert(__builtin_offsetof(UnkNavPolygon_l298968, links) == 0x50, "UnkNavPolygon_l298968 layout");

struct UnkNavPolygonList_l298968 {
    struct Node {
        UnkNavPolygon_l298968* value;
        Node* next;
    };
    u8 pad_0x0[0x10];
    Node* head;
};

static inline bool IsNear(f32 a, f32 b) {
    f32 low = a - 1.0f;
    f32 high = a + 1.0f;
    return low <= b && high >= b;
}

void fn_7100298968(void* self, UnkNavPolygonList_l298968* polygons) {
    for (UnkNavPolygonList_l298968::Node* node = polygons->head; node; node = node->next) {
        UnkNavPolygon_l298968* polygon = node->value;
        for (u32 i = 0; i < polygon->count; ++i)
            polygon->links.Set(i, UnkEdgeLink_l298968{nullptr, 0});
    }
    for (UnkNavPolygonList_l298968::Node* node_a = polygons->head; node_a; node_a = node_a->next) {
        UnkNavPolygon_l298968* a = node_a->value;
        UnkBox2D_l298968 box = a->Bounds();
        box.Extend(CVector2D{box.max.x + 50.0f, box.max.y + 50.0f});
        box.Extend(CVector2D{box.min.x - 50.0f, box.min.y - 50.0f});
        for (UnkNavPolygonList_l298968::Node* node_b = node_a->next; node_b; node_b = node_b->next) {
            UnkNavPolygon_l298968* b = node_b->value;
            if (!box.Overlaps(b->Bounds()))
                continue;
            for (u64 i = 0; i < a->count; ++i) {
                UnkSegment2D edge_a = a->Edge(i);
                for (u32 j = 0; j < b->count; ++j) {
                    if (a->links.At(i).polygon)
                        continue;
                    UnkSegment2D edge_b = b->Edge(j);
                    if (IsNear(edge_a.start.x, edge_b.end.x) && IsNear(edge_a.start.y, edge_b.end.y) &&
                        IsNear(edge_a.end.x, edge_b.start.x) && IsNear(edge_a.end.y, edge_b.start.y)) {
                        a->links.Set(i, UnkEdgeLink_l298968{b, j});
                        b->links.Set(j, UnkEdgeLink_l298968{a, (u32)i});
                        break;
                    }
                }
            }
        }
    }
}
