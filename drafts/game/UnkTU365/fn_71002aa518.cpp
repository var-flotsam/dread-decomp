// Unit 365 (navmesh::CNavMeshGenerator): adds an entry to a fixed list of 128 (48-byte entries kept
// nearest first to a point at +0x180c by fn_71002aa690, a libc++ sort): when the list is full the
// last entry is compared with the new one by squared distance (z taken as 0) and the result code says
// whether it fitted or is now full. Names and layouts are placeholders.
#include "types.h"

struct UnkPushResult_l2aa518 {
    u32 code;
    UnkPushResult_l2aa518() {}
    UnkPushResult_l2aa518(const UnkPushResult_l2aa518& o) : code(o.code) {}
};

struct UnkEntry_l2aa518 {
    u32 field_0x0;
    union {
        f32 x;
    };
    f32 y;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;

    UnkEntry_l2aa518& operator=(const UnkEntry_l2aa518& o) {
        field_0x0 = o.field_0x0;
        x = o.x;
        y = o.y;
        field_0xc = o.field_0xc;
        field_0x10 = o.field_0x10;
        field_0x14 = o.field_0x14;
        field_0x18 = o.field_0x18;
        field_0x1c = o.field_0x1c;
        field_0x20 = o.field_0x20;
        field_0x24 = o.field_0x24;
        field_0x28 = o.field_0x28;
        field_0x2c = o.field_0x2c;
        return *this;
    }
};

struct UnkPoint_l2aa518 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkList_l2aa518 {
    u8 pad_0x0[8];
    u32 count;  // 0x8
    UnkEntry_l2aa518 entries[128];
    UnkPoint_l2aa518 center;  // 0x180c
};

struct UnkCmp_l2aa518 {
    UnkPoint_l2aa518 center;
};

// The sort is std::__sort over these entries with the comparator by reference
// (src/game_s7/fn_71002aa690.cpp defines it with no parameters).
extern "C" void fn_71002aa690();

static inline void Sort(UnkEntry_l2aa518* first, UnkEntry_l2aa518* last, UnkCmp_l2aa518& cmp) {
    reinterpret_cast<void (*)(UnkEntry_l2aa518*, UnkEntry_l2aa518*, UnkCmp_l2aa518&)>(fn_71002aa690)(first, last, cmp);
}

static inline f32 DistSq(const UnkPoint_l2aa518& c, const UnkEntry_l2aa518& e) {
    f32 dx = c.x - e.x;
    f32 dy = c.y - e.y;
    return c.z * c.z + (dx * dx + dy * dy);
}

UnkPushResult_l2aa518 fn_71002aa518(UnkList_l2aa518* self, const UnkEntry_l2aa518* e) {
    u32 n = self->count;
    s32 room = (s32)(128u - n);
    UnkPushResult_l2aa518 result;
    if (room > 0) {
        self->entries[n] = *e;
        self->count = n + 1;
        n += 1;
        result.code = room == 1 ? 3 : 1;
    } else {
        result.code = 2;
        UnkEntry_l2aa518& last = self->entries[n - 1];
        f32 x = last.x;
        f32 da = DistSq(self->center, last);
        f32 db = DistSq(self->center, *e);
        f32 d = da - db;
        if ((s32)(f32)((d > 0.0f) - (d < 0.0f)) > 0)
            last.x = x;
    }
    UnkCmp_l2aa518 cmp = {self->center};
    Sort(self->entries, self->entries + n, cmp);
    return result;
}
