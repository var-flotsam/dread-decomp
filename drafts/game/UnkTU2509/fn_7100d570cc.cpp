// Unit 2509 (collision contacts): a swept query box against an oriented box shape placed and turned by a
// transform. A hit is offered to the shape's filter, then reported to the sink at the shape's depth.
// Names and layouts are placeholders.
#include "base/spatial/COBox2D.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
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
struct UnkSweepContact_d570cc {
    Vec3 point;
    Vec3 normal_a;
    Vec3 normal_b;
    f32 param;
    u32 flags;

    UnkSweepContact_d570cc()
        : point(0.0f, 0.0f, 0.0f), normal_a(0.0f, 0.0f, 0.0f), normal_b(0.0f, 0.0f, 0.0f), param(0.0f), flags(0) {}
    UnkSweepContact_d570cc(const UnkSweepContact_d570cc& o)
        : point(o.point), normal_a(o.normal_a), normal_b(o.normal_b), param(o.param), flags(o.flags) {}
};

// What the sink receives: which part of the shape (-1: all of it), the contact and its flags.
struct UnkSweepHit_d570cc {
    s32 id;
    Vec3 point;
    Vec3 normal_a;
    Vec3 normal_b;
    f32 param;
    u32 flags;

    UnkSweepHit_d570cc(s32 id_, const Vec3& point_, const UnkSweepContact_d570cc& c)
        : id(id_), point(point_), normal_a(c.normal_a), normal_b(c.normal_b), param(c.param), flags(c.flags | 8) {}
    UnkSweepHit_d570cc(const UnkSweepHit_d570cc& o)
        : id(o.id), point(o.point), normal_a(o.normal_a), normal_b(o.normal_b), param(o.param), flags(o.flags) {}
};

struct UnkSweepSink_d570cc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkFlags vf_0x10(UnkSweepHit_d570cc hit);
};

// The contact reported to the sink at `point`, for the whole shape.
inline UnkFlags Report_d570cc(void* sink, const Vec3& point, const UnkSweepContact_d570cc& contact) {
    UnkFlags reply = static_cast<UnkSweepSink_d570cc*>(sink)->vf_0x10(UnkSweepHit_d570cc(-1, point, contact));
    return reply;
}

// The shape's oriented box, copied member by member.
struct UnkOBox_d570cc {
    f32 center_x, center_y;
    f32 axis_x[2];
    f32 axis_y[2];
    f32 size[2];
    f32 offset;
    f32 angle;

    UnkOBox_d570cc(const UnkOBox_d570cc& o)
        : center_x(o.center_x), center_y(o.center_y), axis_x{o.axis_x[0], o.axis_x[1]},
          axis_y{o.axis_y[0], o.axis_y[1]}, size{o.size[0], o.size[1]}, offset(o.offset), angle(o.angle) {}
};

struct UnkSegment2D;
struct UnkOrientedBox2D;
struct UnkContact2D;
bool fn_710012bb00(const UnkSegment2D* segment, const UnkOrientedBox2D* box, UnkContact2D* out);

struct UnkShape40;
typedef UnkFlags (*UnkShapeFilter_d570cc)(void* ctx, const UnkQueryBox* box, UnkShape40* shape, const UnkMtx34* xf,
                                          UnkSweepContact_d570cc* contact);

// The oriented box shape (type 0x40): a filter for its hits, then its box.
struct UnkBoxShape_d570cc {
    u8 pad_0x0[0x58];
    void* filter_ctx;               // 0x58
    UnkShapeFilter_d570cc filter;   // 0x60
    u8 pad_0x68[0x40];
    UnkOBox_d570cc box;  // 0xa8
};

UnkFlags fn_7100d570cc(UnkWorld* self, const UnkQueryBox* box, UnkShape40* shape, const UnkMtx34* xf, void* arg,
                       u32 arg2) {
    UnkBoxShape_d570cc* s = reinterpret_cast<UnkBoxShape_d570cc*>(shape);
    UnkFlags result = NoFlags();
    UnkOBox_d570cc placed(s->box);
    const Vec3 t = xf->GetTranslation();
    placed.center_x = t.x + placed.center_x;
    placed.center_y = t.y + placed.center_y;
    fn_7100110664(reinterpret_cast<base::spatial::COBox2D*>(&placed),
                  fn_7100076acc(reinterpret_cast<const UnkXform*>(xf)).z + placed.angle);
    UnkSweepContact_d570cc contact;
    // The original also passes arg2 to fn_710012bb00, which takes no fourth argument.
    if (fn_710012bb00(reinterpret_cast<const UnkSegment2D*>(box), reinterpret_cast<const UnkOrientedBox2D*>(&placed),
                      reinterpret_cast<UnkContact2D*>(&contact))) {
        UnkSweepContact_d570cc hit = contact;
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
            result = Report_d570cc(arg, Add(hit.point, Vec3(0.0f, 0.0f, depth)), hit);
        }
    }
    return result;
}
