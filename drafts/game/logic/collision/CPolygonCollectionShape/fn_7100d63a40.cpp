// game::logic::collision::CPolygonCollectionShape's unit (tools/tu_map.py unit 2515): runs every edge of
// every polygon of a collection against a shape and reports each contact point found to the sink, until
// the sink asks to stop. Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU365.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64*);
extern "C" void __cxa_guard_release(u64*);

// The result of a contact query: bit 0 keeps a point, bit 1 stops the query.
struct UnkContact_ld63a40 {
    u32 value;

    UnkContact_ld63a40(u32 v) : value(v) {}
    UnkContact_ld63a40(const UnkContact_ld63a40& other) : value(other.value) {}
};

static inline const UnkContact_ld63a40& NoContact_ld63a40() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            reinterpret_cast<UnkContact_ld63a40&>(g_unk_71726a4778).value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return reinterpret_cast<UnkContact_ld63a40&>(g_unk_71726a4778);
}

struct UnkBox2D;
template <u32 N>
struct UnkContactBuffer;

// One contact point (0x2c bytes).
struct UnkPoint_ld63a40 {
    u8 data[0x2c];
};

// What fn_710012b2c0 fills in: a count, then that many points (room for two).
struct UnkContacts_ld63a40 {
    u32 count;
    UnkPoint_ld63a40 points[2];
};

bool fn_710012b2c0(const void* b, const UnkBox2D* a, UnkContactBuffer<2>* out, bool flag);

struct UnkVertex_ld63a40 {
    f32 x, y, z;
};

// One polygon of the collection (0x48 bytes): its vertices and whether the outline is left open.
struct UnkPolygon_ld63a40 {
    const UnkVertex_ld63a40* vertices;
    u32 count;  // 0x8
    u8 pad_0xc[0x20];
    u8 open;  // 0x2c
    u8 pad_0x2d[0x1b];

    // The edge from vertex i to the next one, the last edge closing the outline.
    UnkSegment2D Edge(u64 i) const {
        const UnkVertex_ld63a40* start = &vertices[i];
        const UnkVertex_ld63a40* end = (i == count - 1) ? vertices : start + 1;
        return UnkSegment2D(*reinterpret_cast<const base::math::CVector2D*>(start),
                            *reinterpret_cast<const base::math::CVector2D*>(end));
    }
    // How many edges there are.
    u64 EdgeCount() const { return count - (count != 0 && open == 0); }
};
static_assert(sizeof(UnkPolygon_ld63a40) == 0x48, "UnkPolygon layout");

struct UnkPolygonList_ld63a40 {
    UnkPolygon_ld63a40* data;
    u32 count;
};

// What the sink receives: which polygon and edge, and the contact point.
struct UnkHit_ld63a40 {
    const UnkPolygon_ld63a40* polygon;
    s32 id;
    UnkPoint_ld63a40 point;

    UnkHit_ld63a40(const UnkPolygon_ld63a40* p, s32 id_, const UnkPoint_ld63a40& pt) : polygon(p), id(id_), point(pt) {}
    UnkHit_ld63a40(const UnkHit_ld63a40& o) : polygon(o.polygon), id(o.id), point(o.point) {}
};

struct UnkSink_ld63a40 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkContact_ld63a40 vf_0x10(UnkHit_ld63a40 hit);
};

UnkContact_ld63a40 fn_7100d63a40(UnkPolygonList_ld63a40* self, const void* shape, bool flag, UnkSink_ld63a40* sink) {
    UnkContact_ld63a40 result = NoContact_ld63a40();
    UnkPolygon_ld63a40* end = self->data + self->count;
    s32 polygon_index = 0;
    for (UnkPolygon_ld63a40* poly = self->data; poly != end; ++poly, ++polygon_index) {
        u64 i = 0;
        while (!(result.value & 2)) {
            if (i >= poly->EdgeCount())
                break;
            UnkSegment2D edge = poly->Edge(i);
            UnkContacts_ld63a40 contacts;
            contacts.count = 0;
            bool hit = fn_710012b2c0(&edge, reinterpret_cast<const UnkBox2D*>(shape),
                                     reinterpret_cast<UnkContactBuffer<2>*>(&contacts), flag & 1);
            if (hit && !(result.value & 2)) {
                UnkPoint_ld63a40* p = contacts.points;
                UnkPoint_ld63a40* last = contacts.points + contacts.count;
                if (p != last) {
                    if (__builtin_expect(!sink, 0)) {
                        result.value |= 3;
                    } else {
                        do {
                            result.value |=
                                sink->vf_0x10(UnkHit_ld63a40(poly, (polygon_index << 16) + i, *p)).value;
                            if (result.value & 2)
                                break;
                            ++p;
                        } while (p != last);
                    }
                }
            }
            ++i;
        }
    }
    return result;
}
