// Unit 2509 (collision contacts): a swept query box against a box shape placed by a transform. A hit is
// offered to the shape's filter, then reported to the sink at the shape's depth.
// Names and layouts are placeholders.
#include "game/malloc.h"
#include "types.h"

struct UnkFlags {
    u32 bits;
    UnkFlags() : bits(0) {}
    UnkFlags(const UnkFlags& o) : bits(o.bits) {}
    UnkFlags& operator=(const UnkFlags& o) {
        bits = o.bits;
        return *this;
    }
};

struct Vec3 {
    f32 x, y, z;
    Vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    Vec3(const Vec3& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkMtx34 {
    f32 m[12];
    Vec3 GetTranslation() const { return Vec3{m[3], m[7], m[11]}; }
};

struct UnkWorld;
struct UnkQueryBox;

// The "nothing found" result, shared by the unit's queries.
inline const UnkFlags& NoFlags() {
    static UnkFlags s_none;
    return s_none;
}

static inline UnkFlags MakeFlags(u32 bits) {
    UnkFlags f;
    f.bits = bits;
    return f;
}

static inline Vec3 Add(const Vec3& a, const Vec3& b) {
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

// What the sweep test fills on a hit: the point, two normals, where along the sweep, and which are set.
struct UnkSweepContact_d56b08 {
    Vec3 point;
    Vec3 normal_a;
    Vec3 normal_b;
    f32 param;
    u32 flags;

    UnkSweepContact_d56b08()
        : point(0.0f, 0.0f, 0.0f), normal_a(0.0f, 0.0f, 0.0f), normal_b(0.0f, 0.0f, 0.0f), param(0.0f), flags(0) {}
    UnkSweepContact_d56b08(const UnkSweepContact_d56b08& o)
        : point(o.point), normal_a(o.normal_a), normal_b(o.normal_b), param(o.param), flags(o.flags) {}
};

// What the sink receives: which part of the shape (-1: all of it), the contact and its flags.
struct UnkSweepHit_d56b08 {
    s32 id;
    Vec3 point;
    Vec3 normal_a;
    Vec3 normal_b;
    f32 param;
    u32 flags;

    UnkSweepHit_d56b08(s32 id_, const Vec3& point_, const UnkSweepContact_d56b08& c)
        : id(id_), point(point_), normal_a(c.normal_a), normal_b(c.normal_b), param(c.param), flags(c.flags | 8) {}
    UnkSweepHit_d56b08(const UnkSweepHit_d56b08& o)
        : id(o.id), point(o.point), normal_a(o.normal_a), normal_b(o.normal_b), param(o.param), flags(o.flags) {}
};

struct UnkSweepSink_d56b08 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkFlags vf_0x10(UnkSweepHit_d56b08 hit);
};

// The contact reported to the sink at `point`, for the whole shape.
inline UnkFlags Report_d56b08(void* sink, const Vec3& point, const UnkSweepContact_d56b08& contact) {
    UnkFlags reply = static_cast<UnkSweepSink_d56b08*>(sink)->vf_0x10(UnkSweepHit_d56b08(-1, point, contact));
    return reply;
}

struct UnkBox_d56b08 {
    f32 min_x, min_y, max_x, max_y;

    UnkBox_d56b08(f32 x0, f32 y0, f32 x1, f32 y1) : min_x(x0), min_y(y0), max_x(x1), max_y(y1) {}
};

struct UnkShape4;
typedef UnkFlags (*UnkShapeFilter_d56b08)(void* ctx, const UnkQueryBox* box, UnkShape4* shape, const UnkMtx34* xf,
                                          UnkSweepContact_d56b08* contact);

// The box shape (type 4): a filter for its hits, then its two corners.
struct UnkBoxShape_d56b08 {
    u8 pad_0x0[0x58];
    void* filter_ctx;               // 0x58
    UnkShapeFilter_d56b08 filter;   // 0x60
    u8 pad_0x68[0x40];
    f32 min_x, min_y;  // 0xa8
    f32 max_x, max_y;  // 0xb0
};

UnkFlags fn_7100d56b08(UnkWorld* self, const UnkQueryBox* box, UnkShape4* shape, const UnkMtx34* xf, void* arg,
                       u32 arg2) {
    UnkBoxShape_d56b08* s = reinterpret_cast<UnkBoxShape_d56b08*>(shape);
    UnkFlags result = NoFlags();
    const Vec3 t = xf->GetTranslation();
    UnkBox_d56b08 placed(t.x + s->min_x, t.y + s->min_y, t.x + s->max_x, t.y + s->max_y);
    UnkSweepContact_d56b08 contact;
    if (fn_710012b7f0(reinterpret_cast<const UnkSweep2D*>(box), reinterpret_cast<const UnkBox2D*>(&placed),
                      reinterpret_cast<UnkContact2D*>(&contact), arg2)) {
        UnkSweepContact_d56b08 hit = contact;
        if (!arg) {
            result = MakeFlags(3);
        } else {
            f32 depth = xf->GetTranslation().z;
            if (s->filter) {
                UnkFlags verdict = s->filter(s->filter_ctx, box, shape, xf, &hit);
                if (!(verdict.bits & 1)) {
                    result = verdict;
                    return result;
                }
            }
            result = Report_d56b08(arg, Add(hit.point, Vec3(0.0f, 0.0f, depth)), hit);
        }
    }
    return result;
}
