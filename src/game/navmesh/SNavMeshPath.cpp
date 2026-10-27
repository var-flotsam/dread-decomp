// navmesh::SNavMeshPath's unit (game/UnkNavMeshPath.h): fn_71002ac624 appends a point to the
// path's fixed array of up to 250 points (0x120 bytes each, from +0xc, the count at +0x8) and marks
// the path changed. Names and layouts are placeholders.
#include <new>

#include "game/UnkNavMeshPath.h"
#include "game/UnkObject_0x100.h"
#include "types.h"

// A point's position, copied word by word.
struct UnkNavMeshPathPos {
    u32 x, y, z;

    UnkNavMeshPathPos(const UnkNavMeshPathPos& other) : x(other.x), y(other.y), z(other.z) {}
};

// Where the point lies (copied as three bytes).
struct UnkNavMeshPathPlace {
    u16 field_0x0;
    u8 field_0x2;

    UnkNavMeshPathPlace(u16 a, u8 b) : field_0x0(a), field_0x2(b) {}
    UnkNavMeshPathPlace(const UnkNavMeshPathPlace& other) { __builtin_memcpy(this, &other, 3); }
};

// A point of the path: where it lies (a 16-bit and an 8-bit word), its position, a 0x100-byte
// payload and two flags. Polymorphic (its vtable is set by hand: the array places points 4 bytes off
// alignment).
struct UnkNavMeshPathPoint {
    void* vtable;
    UnkNavMeshPathPlace place;
    UnkNavMeshPathPos pos;
    UnkObject_0x100 data;
    bool field_0x118;
    bool field_0x119;

    UnkNavMeshPathPoint(u16 a, u8 b, const UnkNavMeshPathPos& pos_, const UnkObject_0x100& data_, bool f0, bool f1)
        : vtable(g_unk_7101a21120 + 2), place(a, b), pos(pos_), data(data_), field_0x118(f0), field_0x119(f1) {}
    UnkNavMeshPathPoint(const UnkNavMeshPathPoint& other)
        : vtable(g_unk_7101a21120 + 2), place(other.place), pos(other.pos), data(other.data) {
        // The two flags are copied together.
        __builtin_memcpy(&field_0x118, &other.field_0x118, 2);
    }
    ~UnkNavMeshPathPoint() { vtable = g_unk_7101a21120 + 2; }
};
static_assert(sizeof(UnkNavMeshPathPoint) == 0x120, "UnkNavMeshPathPoint layout");

// The points (from +0x8 of the path) and the changed flag after them (+0x1194c).
struct UnkNavMeshPathPoints {
    u32 count;
    u8 storage[250][0x120];
    bool changed;

    void PushBack(const UnkNavMeshPathPoint& point) {
        new (storage[count]) UnkNavMeshPathPoint(point);
        ++count;
    }
};

static inline UnkNavMeshPathPoints* Points(UnkNavMeshPath* path) {
    return reinterpret_cast<UnkNavMeshPathPoints*>(reinterpret_cast<u8*>(path) + 8);
}

// Appends a point and marks the path changed.
void fn_71002ac624(UnkNavMeshPath* self, u16 a, u8 b, const UnkNavMeshPathPos* pos, const UnkObject_0x100* data,
                   bool f0, bool f1) {
    UnkNavMeshPathPoint point(a, b, *pos, *data, f0, f1);
    UnkNavMeshPathPoints* points = Points(self);
    points->PushBack(point);
    points->changed = true;
}
