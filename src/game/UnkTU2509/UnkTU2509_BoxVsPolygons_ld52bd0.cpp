// Unit 2509 (collision contacts): a polygon collection shape against an axis-aligned box shape. The
// box is moved into the collection's frame and run through the collection's tree; when that finds
// nothing, each polygon is tried by its first vertex (inside the box?) and by its outline around the
// box's centre. Names and layouts are placeholders.
#include "game/UnkTU108.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game/logic/collision/CShape.h"
#include "types.h"

struct UnkCollider;
struct UnkShape;
struct UnkShape10;
struct UnkBox2D;
struct UnkLocalQuery;
struct UnkEntry;
struct UnkCollisionShape;
struct UnkPolygonCollectionShape_b42a3;
struct UnkBoxShape2D_7100d5e570;

// The result of a contact query: bit 0 keeps a point, bit 1 stops the query.
struct UnkContact {
    u32 value;

    UnkContact() { value = 0; }
    UnkContact(const UnkContact& other) { value = other.value; }
};

struct UnkVisitResult_b0040;
extern UnkVisitResult_b0040 g_unk_71726a4778;  // the shared "nothing" result (a UnkContact here)
extern u64 g_unk_71726a4780;                   // its guard
extern "C" int __cxa_guard_acquire(u64*);
extern "C" void __cxa_guard_release(u64*);

static inline const UnkContact& NoContact() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            reinterpret_cast<UnkContact&>(g_unk_71726a4778).value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return reinterpret_cast<UnkContact&>(g_unk_71726a4778);
}

static inline UnkContact MakeContact(u32 value) {
    UnkContact c;
    c.value = value;
    return c;
}

// A 2D point, copied member by member.
struct UnkPoint2D_d52bd0 {
    f32 x, y;

    UnkPoint2D_d52bd0(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkPoint2D_d52bd0(const UnkPoint2D_d52bd0& o) : x(o.x), y(o.y) {}
};

// The sum of two vectors. Not static, so that each component is read where it is added.
inline UnkVec3f Sum_d52bd0(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

// A 2D point on the collision plane as a 3D position.
static inline UnkVec3f ToVec3(const UnkPoint2D_d52bd0& p) {
    return UnkVec3f(p.x, p.y, 0.0f);
}

// One polygon of the collection (0x48 bytes).
struct UnkPolygon_d52bd0 {
    const UnkPoint2D_d52bd0* points;
    u8 pad_0x8[0x20];
    f32 height;  // 0x28
    u8 field_0x2c;
    u8 pad_0x2d[0x1b];
};
static_assert(sizeof(UnkPolygon_d52bd0) == 0x48, "UnkPolygon layout");

// What the sink receives: which polygon (index in the high half), where, and flags.
struct UnkContactHit {
    s32 id;
    UnkVec3f pos;
    UnkVec3f field_0x10;
    UnkVec3f field_0x1c;
    u32 field_0x28;
    u32 flags;

    UnkContactHit(s32 id_, const UnkVec3f& pos_)
        : id(id_), pos(pos_), field_0x10(0.0f, 0.0f, 0.0f), field_0x1c(0.0f, 0.0f, 0.0f), field_0x28(0), flags(0x39) {}
    UnkContactHit(const UnkContactHit& o)
        : id(o.id), pos(o.pos), field_0x10(o.field_0x10), field_0x1c(o.field_0x1c), field_0x28(o.field_0x28),
          flags(o.flags) {}
};
static_assert(sizeof(UnkContactHit) == 0x30, "UnkContactHit layout");

struct UnkContactSink {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkContact vf_0x10(UnkContactHit hit);
};

extern char g_unk_7101aa8870[];  // the local query's vtable
extern char g_unk_7101aa88c0[];  // CAABoxShape2D's vtable

// The query the collection runs for a shape placed in its frame: where to report the points, the
// collection's polygons and the sink.
struct UnkLocalQuery_d52bd0 {
    const void* vtable;
    UnkVec3f offset;
    u16 flags;
    const void* polygons;
    UnkContactSink* sink;

    UnkLocalQuery_d52bd0(const UnkVec3f& o, u16 f, const void* p, UnkContactSink* s)
        : vtable(g_unk_7101aa8870 + 0x10), offset(o), flags(f), polygons(p), sink(s) {}

    // One polygon reported at `at` (moved by the query's offset and lifted to the middle of the
    // polygon's height); 1 without a sink.
    UnkContact Report(s32 id, const UnkVec3f& at, const UnkPolygon_d52bd0& poly) {
        if (sink) {
            return sink->vf_0x10(
                UnkContactHit(id, Sum_d52bd0(Sum_d52bd0(at, offset), UnkVec3f(0.0f, 0.0f, poly.height * 0.5f))));
        }
        return MakeContact(1);
    }
};

void fn_7100d5e570(UnkBoxShape2D_7100d5e570* self, const f32* delta);
struct UnkBoxShape_d52bd0;
bool fn_7100d5e5b4(const UnkBoxShape_d52bd0* self, UnkPoint2D_d52bd0 point);
UnkPoint2D_d52bd0 fn_7100d5e540(const UnkBoxShape_d52bd0* self);

struct UnkShapeBase_d52bd0 {
    const void* vtable;
    u8 pad_0x8[0xa0];
};

// The box shape (type 4): the base shape, then its two corners.
struct UnkBoxShape_d52bd0 : UnkShapeBase_d52bd0 {
    f32 min_x, min_y;  // 0xa8
    f32 max_x, max_y;  // 0xb0
    u8 field_0xb8;

    UnkBoxShape_d52bd0(const UnkBoxShape_d52bd0& o) {
        fn_7100d6c0e4(reinterpret_cast<UnkPolygonCollectionShape_b42a3*>(this),
                      reinterpret_cast<const UnkPolygonCollectionShape_b42a3*>(&o));
        vtable = g_unk_7101aa88c0 + 0x10;
        min_x = o.min_x;
        min_y = o.min_y;
        max_x = o.max_x;
        max_y = o.max_y;
        field_0xb8 = o.field_0xb8;
    }
    ~UnkBoxShape_d52bd0() { fn_7100d5e2c8(reinterpret_cast<UnkCollisionShape*>(this)); }

    void Move(const UnkPoint2D_d52bd0& delta) {
        fn_7100d5e570(reinterpret_cast<UnkBoxShape2D_7100d5e570*>(this), &delta.x);
    }
    bool Contains(const UnkPoint2D_d52bd0& point) const { return fn_7100d5e5b4(this, point); }
    UnkPoint2D_d52bd0 Corner() const { return fn_7100d5e540(this); }
    UnkPoint2D_d52bd0 Centre() const { return UnkPoint2D_d52bd0((min_x + max_x) * 0.5f, (min_y + max_y) * 0.5f); }
};

struct UnkPolygonList_d52bd0 {
    UnkPolygon_d52bd0* data;
    u32 count;

    UnkPolygon_d52bd0* begin() const { return data; }
    UnkPolygon_d52bd0* end() const { return data + count; }
};

struct UnkCollectionShape_d52bd0 {
    u8 pad_0x0[0x98];
    u8 field_0x98;
    u8 pad_0x99[0xf];
    UnkPolygonList_d52bd0 polygons;  // 0xa8
};

UnkContact fn_7100d63870(UnkShape10* self, const UnkBox2D* box, UnkLocalQuery* query, bool flag);

UnkContact fn_7100d52bd0(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* sink, u32 x6) {
    UnkCollectionShape_d52bd0* collection = reinterpret_cast<UnkCollectionShape_d52bd0*>(a);
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    UnkContact result = NoContact();
    // The option bits (bit 0: only what the tree finds is wanted). The original seems to receive them
    // as a small type of their own; the shared declaration of this function passes a u32.
    u16 options = x6;
    // keeps the option bits a 16-bit value, so their test stays apart from the result's
    asm("" : "+r"(options));

    // The box, moved to where the collection sees it.
    const UnkVec3f pa = xa->GetTranslation();
    const UnkVec3f pb = xb->GetTranslation();
    const f32 dx = pa.x - pb.x, dy = pa.y - pb.y;
    UnkBoxShape_d52bd0 local(*reinterpret_cast<const UnkBoxShape_d52bd0*>(b));
    local.Move(UnkPoint2D_d52bd0(-dx, -dy));

    // Contacts are reported around the collection's position, halfway between the two in depth.
    UnkVec3f origin = xa->GetTranslation();
    origin.z = (origin.z + xb->GetTranslation().z) * 0.5f;
    UnkLocalQuery_d52bd0 query(origin, options, &collection->polygons, static_cast<UnkContactSink*>(sink));
    result = fn_7100d63870(reinterpret_cast<UnkShape10*>(collection), reinterpret_cast<const UnkBox2D*>(&local.min_x),
                           reinterpret_cast<UnkLocalQuery*>(&query), false);
    if (!(options & 1) && !(result.value & 3)) {
        UnkPoint2D_d52bd0 centre = local.Centre();
        u32 index = 0;
        for (UnkPolygon_d52bd0* poly = collection->polygons.begin(); poly != collection->polygons.end();
             ++poly, ++index) {
            if (local.Contains(*poly->points)) {
                if (!sink)
                    return MakeContact(3);
                UnkVec3f at = Sum_d52bd0(ToVec3(*poly->points), origin);
                result.value = query.Report(index << 16, at, *poly).value | result.value;
            } else if (collection->field_0x98 && poly->field_0x2c &&
                       fn_7100116de0(reinterpret_cast<UnkEntry*>(poly), &centre, 1, 1, 1.0f)) {
                if (!sink)
                    return MakeContact(3);
                UnkVec3f at = Sum_d52bd0(ToVec3(local.Corner()), origin);
                result.value = query.Report(index << 16, at, *poly).value | result.value;
                break;
            }
        }
    }
    return result;
}
