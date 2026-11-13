// Unit 365 (navmesh::CNavMeshGenerator?): collects every pair of linked polygon edges that no box
// already covers, longest first, then connects each pair the check accepts.
// Names and layouts are placeholders.

#include "base/UnkNoArg.h"
#include "base/UnkVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

struct UnkCachedSegment2D;
f32 fn_7100126238(UnkCachedSegment2D* self);

extern char g_unk_7101a210d8[];



struct UnkNavVertex {
    base::math::CVector2D xy;
    f32 z;
};

struct UnkNavPoly;

struct UnkNavLink {
    UnkNavPoly* poly;
    u32 edge;
    UnkNavLink* next;
};

struct UnkNavPoly {
    UnkNavVertex* vertices;  // 0x0
    u32 vertexCount;         // 0x8
    u8 pad_0xc[0x44];
    u32 edgeCount;           // 0x50
    UnkNavLink* links;       // 0x58

    UnkNavLink* LinkAt(u64 index) {
        for (UnkNavLink* n = links; n; n = n->next) {
            if (index-- == 0)
                return n;
        }
        return nullptr;
    }

    // The far end of edge `i` (its segment's end point).
    base::math::CVector2D EdgeEnd(u64 i) {
        UnkSegment2D seg(vertices[i].xy, (i == vertexCount - 1 ? vertices[0] : vertices[i + 1]).xy);
        return seg.end;
    }
};

struct UnkNavPolyNode {
    UnkNavPoly* poly;
    UnkNavPolyNode* next;
};

struct UnkNavPolyList {
    u8 pad_0x0[0x10];
    UnkNavPolyNode* head;  // 0x10
};

struct UnkNavBox {
    u8 pad_0x0[0xc];
    base::math::CVector2D p0;  // 0xc
    u8 pad_0x14[0x10];
    base::math::CVector2D p1;  // 0x24
    u8 pad_0x2c[0x4];
    UnkNavBox* next;           // 0x30
};

struct UnkNavBoxList {
    u8 pad_0x0[0x10];
    UnkNavBox* head;  // 0x10
};

struct UnkEdgePair {
    UnkNavPoly* a;
    u32 edgeA;
    UnkNavPoly* b;
    u32 edgeB;
    f32 lengthSq;
};

struct UnkEdgePairList {
    struct Node {
        UnkEdgePair value;
        Node* next;
    };
    void* vtable;
    u32 count;
    Node* head;
    Node* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkEdgePairList() {
        count = 0;
        head = nullptr;
        tail = nullptr;
        vtable = g_unk_7101a210d8 + 0x10;
        allocator = fn_7100083990();
        flags = 0x13;
    }

    ~UnkEdgePairList() {
        vtable = g_unk_7101a210d8 + 0x10;
        for (Node* n = head; n;) {
            Node* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }

    __attribute__((always_inline)) Node* NewNode(const UnkEdgePair& v) {
        Node* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, flags));
        if (n) {
            n->value = v;
            n->next = nullptr;
        }
        return n;
    }

    __attribute__((always_inline)) void PushFront(const UnkEdgePair& v) {
        Node* n = NewNode(v);
        if (!n)
            return;
        if (count == 0) {
            head = tail = n;
            count = 1;
        } else {
            n->next = head;
            head = n;
            ++count;
        }
    }

    __attribute__((always_inline)) void PushBack(const UnkEdgePair& v) {
        Node* n = NewNode(v);
        if (!n)
            return;
        if (count == 0) {
            head = tail = n;
            count = 1;
        } else {
            tail->next = n;
            n->next = nullptr;
            tail = n;
            ++count;
        }
    }

    __attribute__((always_inline)) void InsertAfter(Node* prev, const UnkEdgePair& v) {
        if (!prev) {
            PushFront(v);
            return;
        }
        if (count == 0) {
            PushBack(v);
            return;
        }
        Node* n = NewNode(v);
        if (!n)
            return;
        Node* next = prev->next;
        prev->next = n;
        n->next = next;
        if (!next)
            tail = n;
        ++count;
    }

    __attribute__((always_inline)) void InsertBefore(const UnkEdgePair& v, Node* pos) {
        Node* prev = nullptr;
        for (Node* it = head; it; prev = it, it = it->next)
            if (it == pos) {
                InsertAfter(prev, v);
                return;
            }
    }

    __attribute__((always_inline)) void Remove(Node* node) {
        Node* prev = nullptr;
        for (Node* it = head; it; prev = it, it = it->next) {
            if (it != node)
                continue;
            if (prev) {
                prev->next = it->next;
                if (tail == it)
                    tail = prev;
            } else {
                head = it->next;
            }
            if (--count == 0)
                head = tail = nullptr;
            allocator->vf_0x48(it);
            return;
        }
    }

    // Index of the pair joining these two edges, in either order, or -1.
    __attribute__((always_inline)) s32 IndexOf(UnkNavPoly* poly, u64 edge, UnkNavPoly* other, u32 otherEdge) {
        s32 index = 0;
        for (Node* n = head; n; n = n->next, ++index) {
            const UnkEdgePair& p = n->value;
            if ((p.a == poly && p.edgeA == edge && p.b == other && p.edgeB == otherEdge) ||
                (p.a == other && p.edgeA == otherEdge && p.b == poly && p.edgeB == edge))
                return index;
        }
        return -1;
    }
};

static inline bool Near(f32 p, f32 v) {
    return p + 1.0f >= v && p + -1.0f <= v;
}

static inline bool Near(const base::math::CVector2D& p, const base::math::CVector2D& v) {
    return Near(p.x, v.x) && Near(p.y, v.y);
}



bool fn_71002a25f0(UnkNoArg, UnkNavPoly* a, u32 edgeA, UnkNavPoly* b, u32 edgeB);
void fn_71002a28f4(void* self, UnkNavPoly* a, u32 edgeA, UnkNavPoly* b, u32 edgeB, UnkNavPolyList* polys,
                   UnkEdgePairList* pairs);

void fn_7100298e08(void* self, UnkNavBoxList* boxes, UnkNavPolyList* polys) {
    UnkEdgePairList pairs;
    for (UnkNavPolyNode* it = polys->head; it; it = it->next) {
        UnkNavPoly* poly = it->poly;
        for (u64 i = 0; i < poly->edgeCount; ++i) {
            UnkNavLink* link = poly->LinkAt(i);
            if (!link)
                continue;
            UnkNavPoly* other = link->poly;
            if (!other)
                continue;
            u32 otherEdge = link->edge;
            if (pairs.IndexOf(poly, i, other, otherEdge) >= 0)
                continue;

            UnkSegment2D seg(poly->vertices[i].xy,
                             (i == poly->vertexCount - 1 ? poly->vertices[0] : poly->vertices[i + 1]).xy);
            f32 lengthSq = fn_7100126238(reinterpret_cast<UnkCachedSegment2D*>(&seg));

            bool covered = false;
            for (UnkNavBox* box = boxes->head; box; box = box->next) {
                base::math::CVector2D endA = poly->EdgeEnd(i);
                base::math::CVector2D endB = other->EdgeEnd(otherEdge);
                if ((Near(box->p0, endA) && Near(box->p1, endB)) || (Near(box->p0, endB) && Near(box->p1, endA)))
                    covered = true;
            }
            if (covered)
                continue;

            UnkEdgePair pair = {poly, (u32)i, other, otherEdge, lengthSq};
            UnkEdgePairList::Node* pos = pairs.head;
            while (pos && !(pos->value.lengthSq < lengthSq))
                pos = pos->next;
            if (pos)
                pairs.InsertBefore(pair, pos);
            else
                pairs.PushBack(pair);
        }
    }

    for (UnkEdgePairList::Node* n = pairs.head; n;) {
        UnkEdgePair& p = n->value;
        if (fn_71002a25f0(UnkNoArg(), p.a, p.edgeA, p.b, p.edgeB)) {
            fn_71002a28f4(self, p.a, p.edgeA, p.b, p.edgeB, polys, &pairs);
            pairs.Remove(n);
            n = pairs.head;
        } else {
            n = n->next;
        }
    }
}
