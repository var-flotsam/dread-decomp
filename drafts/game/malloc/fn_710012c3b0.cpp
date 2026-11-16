// malloc unit (tools/tu_map.py unit 110): a segment against every edge of a set of 2D paths, adding
// each contact to a list of at most 16. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPolygonVertex {
    CVector2D pos;
    f32 field_0x8;
};

// One path: its vertices, their count and whether it closes on itself.
struct UnkPath2D {
    UnkPolygonVertex* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    bool closed;
    u8 unk_0x2d[0x1b];

    u32 PointCount() const { return count - (!closed & (count != 0)); }
    u32 EdgeCount() const {
        u32 n = PointCount();
        return n - ((n < 3) | !closed);
    }
};
static_assert(sizeof(UnkPath2D) == 0x48, "UnkPath2D layout");

struct UnkPathSet2D {
    UnkPath2D* paths;
    u32 count;
};

// A contact as fn_710012be70 fills it; copied member by member.
struct UnkPathContact {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    u32 field_0x24;
    u32 flags;

    UnkPathContact& operator=(const UnkPathContact& o) {
        point.x = o.point.x;
        point.y = o.point.y;
        point.z = o.point.z;
        normal_a.x = o.normal_a.x;
        normal_a.y = o.normal_a.y;
        normal_a.z = o.normal_a.z;
        normal_b.x = o.normal_b.x;
        normal_b.y = o.normal_b.y;
        normal_b.z = o.normal_b.z;
        field_0x24 = o.field_0x24;
        flags = o.flags;
        return *this;
    }
};
static_assert(sizeof(UnkPathContact) == 0x2c, "UnkPathContact layout");

// The list this test fills: a count and raw storage for 16 contacts.
struct UnkContactList16 {
    u32 count;
    alignas(4) u8 storage[16 * sizeof(UnkPathContact)];

    UnkPathContact* Data() { return reinterpret_cast<UnkPathContact*>(storage); }

    void Add(const UnkPathContact& c) {
        u32 n = count;
        if (n < 16) {
            Data()[n] = c;
            count = n + 1;
        }
    }
};

struct UnkContactList2D;
struct UnkContact_710012be70;
bool fn_710012be70(const UnkSegment2D* a, const UnkSegment2D* b, UnkContact_710012be70* out, bool collinear,
                   f32 tolerance);

bool fn_710012c3b0(const UnkSegment2D* seg, const UnkPathSet2D* paths, UnkContactList2D* list, bool) {
    UnkContactList16* contacts = reinterpret_cast<UnkContactList16*>(list);
    bool hit = false;
    u64 count = paths->count;
    for (u64 i = 0; i < count; ++i) {
        const UnkPath2D* path = &paths->paths[i];
        u32 n = path->PointCount();
        u32 edges = path->EdgeCount();
        for (u64 j = 0; j < edges; ++j) {
            UnkPathContact contact = {};
            UnkSegment2D edge(path->vertices[j].pos, path->vertices[(u32)(j + 1) % n].pos);
            bool edge_hit = fn_710012be70(seg, &edge, reinterpret_cast<UnkContact_710012be70*>(&contact), false,
                                          0.0f);
            hit |= edge_hit;
            if (edge_hit)
                contacts->Add(contact);
        }
    }
    return hit;
}
