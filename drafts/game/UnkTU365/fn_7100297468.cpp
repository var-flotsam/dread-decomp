// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator): classifies every vertex of a region's closed
// polygons by its neighbours (both below, both above, or one of each, with the turn given by the summed
// edge normals) and inserts it into a list sorted top to bottom. Names and layouts are placeholders.
#include <math.h>
#include <new>

#include "base/UnkNoArg.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::math::operator+;

struct UnkNavVertex_l297468 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkNavPolygon_l297468 {
    UnkNavVertex_l297468* vertices;
    u32 count;
    u8 pad_0xc[0x1c];
    f32 height;  // 0x28
    u8 closed;   // 0x2c
    u8 pad_0x2d[0x1b];

    u32 EdgeCount() const { return count - (!closed & (count != 0)); }
    UnkSegment2D Edge(u64 i) const {
        return UnkSegment2D(vertices[i].pos, count - 1 == i ? vertices[0].pos : vertices[i + 1].pos);
    }
};
static_assert(sizeof(UnkNavPolygon_l297468) == 0x48, "UnkNavPolygon_l297468 layout");

struct UnkNavPolygons_l297468 {
    UnkNavPolygon_l297468* data;
    u32 count;
};

struct UnkNavVertexRecord_l297468 {
    CVector2D point;
    UnkNavPolygon_l297468* polygon;
    u32 index;
    u32 kind;
};

// A doubly linked list of records, its nodes from an allocator.
struct UnkNavVertexList_l297468 {
    struct Node {
        UnkNavVertexRecord_l297468 value;
        Node* next;
        Node* prev;
    };
    u64 field_0x0;
    u32 count;
    Node* head;
    Node* tail;
    u32 flags;
    base::IAllocator* allocator;

    Node* NewNode(const UnkNavVertexRecord_l297468& v) {
        Node* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, flags));
        if (n) {
            new (&n->value) UnkNavVertexRecord_l297468(v);
            n->next = nullptr;
            n->prev = nullptr;
        }
        return n;
    }

    void PushBack(const UnkNavVertexRecord_l297468& v) {
        Node* n = NewNode(v);
        if (!n)
            return;
        if (count == 0) {
            head = tail = n;
            count = 1;
        } else {
            tail->next = n;
            n->prev = tail;
            tail = n;
            ++count;
        }
    }

    void InsertBefore(Node* pos, const UnkNavVertexRecord_l297468& v) {
        Node* n = NewNode(v);
        if (!n)
            return;
        Node* prev = pos->prev;
        pos->prev = n;
        n->next = pos;
        n->prev = prev;
        if (prev == nullptr)
            head = n;
        else
            prev->next = n;
        ++count;
    }
};

// Whether `a` comes before `b` top to bottom: by x when their heights are within a tenth.
static inline bool Before(const CVector2D& a, const CVector2D& b) {
    if (a.y + 0.1f >= b.y && a.y - 0.1f <= b.y)
        return a.x > b.x;
    return a.y < b.y;
}

// The edge's unit normal, in the plane.
static inline CVector3D Normal3D(const UnkSegment2D& edge) {
    UnkVec2Result normal = fn_7100125e78(&edge);
    return CVector3D{normal.x, normal.y, 0.0f};
}

void fn_7100297468(u8* region, UnkNoArg, u8* part_a, u8* part_b) {
    UnkNavPolygons_l297468* polygons = reinterpret_cast<UnkNavPolygons_l297468*>(part_a);
    UnkNavVertexList_l297468* list = reinterpret_cast<UnkNavVertexList_l297468*>(part_b);
    UnkNavPolygon_l297468* end = polygons->data + polygons->count;
    for (UnkNavPolygon_l297468* polygon = polygons->data; polygon != end; ++polygon) {
        if (polygon->closed) {
            for (u64 i = 0; i < polygon->EdgeCount(); ++i) {
                u32 next = i + 1 < polygon->EdgeCount() ? i + 1 : 0;
                UnkSegment2D edge = polygon->Edge(i);
                UnkSegment2D next_edge = polygon->Edge(next);
                UnkNavVertexRecord_l297468 record;
                {
                    CVector3D corner = {edge.end.x, edge.end.y, polygon->height};
                    fn_710007b430(&record.point, &corner);
                }
                record.polygon = polygon;
                record.index = i;
                record.kind = 4;
                bool prev_before = Before(record.point, edge.start);
                bool next_before = Before(record.point, next_edge.end);
                CVector3D sum = Normal3D(edge) + Normal3D(next_edge);
                f32 turn = base::math::Dot(base::math::Normalized(sum), CVector3D{0.0f, -1.0f, 0.0f});
                if (prev_before && next_before)
                    record.kind = turn > 0.0f ? 2 : 3;
                else if (!prev_before && !next_before)
                    record.kind = turn > 0.0f ? 0 : 1;

                if (list->count == 0) {
                    list->PushBack(record);
                    continue;
                }
                UnkNavVertexList_l297468::Node* node = list->head;
                while (node && !Before(node->value.point, record.point))
                    node = node->next;
                if (node)
                    list->InsertBefore(node, record);
                else
                    list->PushBack(record);
            }
        } else {
            // The original builds an open chain's first and last edges and uses neither.
            polygon->Edge(0);
            polygon->Edge(polygon->EdgeCount() - 1);
        }
    }
}
