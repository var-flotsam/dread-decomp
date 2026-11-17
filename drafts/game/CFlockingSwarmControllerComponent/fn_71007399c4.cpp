// CFlockingSwarmControllerComponent unit: how far the swarm can spread around a point: sixteen rays of
// twice the swarm's radius are cast from it against the owner's collision shape (fn_7100d56604), and
// the nearest hit, when nearer than the radius, becomes the result. Names and layouts are placeholders.
#include <math.h>
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// As src/game_hi/fn_7100d56604.cpp defines it.
struct UnkFlags {
    u32 bits;
    UnkFlags() : bits(0) {}
    UnkFlags(const UnkFlags& o) : bits(o.bits) {}
    UnkFlags& operator=(const UnkFlags& o) {
        bits = o.bits;
        return *this;
    }
};
struct UnkWorld;
struct UnkQueryBox;
struct UnkShape;
struct UnkMtx34;
UnkFlags fn_7100d56604(UnkWorld* self, const UnkQueryBox* box, UnkShape* shape, const UnkMtx34* xf, void* arg, u32 arg2);

extern void* g_unk_7101a23080[];
// The sixteen directions the rays are cast in.
__attribute__((visibility("hidden"))) extern const CVector2D g_unk_710164a968[16];

// The ray: its start and end, and the end relative to the start.
struct UnkRay_l7399c4 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkRay_l7399c4(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// What the query found: where the ray hit first.
struct UnkHitInfo_l7399c4 {
    CVector3D pos;
    CVector3D field_0xc;
    f32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
};
static_assert(sizeof(UnkHitInfo_l7399c4) == 0x2c, "layout");

// The nearest-hit collector the query reports to.
struct UnkRayHit_l7399c4 {
    void** vtable;
    s32 field_0x8;
    UnkHitInfo_l7399c4 info;  // 0xc
    bool hit;  // 0x38
    u8 pad_0x39[7];
    CVector2D origin;  // 0x40
    u32 field_0x48;
    void* field_0x50;

    explicit UnkRayHit_l7399c4(const CVector2D& from)
        : vtable(g_unk_7101a23080 + 2), field_0x8(-1), info(), hit(false), field_0x48(0),
          field_0x50(nullptr) {
        origin.x = from.x;
        origin.y = from.y;
    }
};
static_assert(sizeof(UnkRayHit_l7399c4) == 0x58, "layout");

struct UnkFlockingSwarm_l7399c4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual UnkShape* vf_0x1b8();  // the collision shape
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
    virtual void vf_0x230();
    virtual void vf_0x238();
    virtual void vf_0x240();
    virtual void vf_0x248();
    virtual void vf_0x250();
    virtual void vf_0x258();
    virtual void vf_0x260();
    virtual void vf_0x268();
    virtual void vf_0x270();
    virtual void vf_0x278();
    virtual void vf_0x280();
    virtual void vf_0x288();
    virtual void vf_0x290();
    virtual void vf_0x298();
    virtual void vf_0x2a0();
    virtual void vf_0x2a8();
    virtual void vf_0x2b0();
    virtual void vf_0x2b8();
    virtual void vf_0x2c0();
    virtual void vf_0x2c8();
    virtual void vf_0x2d0();
    virtual f32 vf_0x2d8();  // the radius
};

static inline UnkShapeTester_A5_b68* Tester_l7399c4() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

static inline UnkMatrix44 Identity_l7399c4() {
    UnkMatrix44 m;
    for (u32 i = 0; i < 16; ++i)
        m.m[i] = 0.0f;
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    return m;
}

// The query takes the transform as fn_7100d56604 declares it.
static inline const UnkMtx34* AsMtx34_l7399c4(const UnkMatrix44& m) {
    return reinterpret_cast<const UnkMtx34*>(&m);
}

f32 fn_71007399c4(UnkFlockingSwarm_l7399c4* self, const CVector2D* pos) {
    f32 radius = self->vf_0x2d8();
    f32 length = radius + radius;
    UnkShape* shape = self->vf_0x1b8();
    s32 nearest = -1;
    f32 distances[16];
    for (u32 i = 0; i < 16; ++i) {
        const CVector2D& dir = g_unk_710164a968[i];
        CVector2D end;
        end.x = length * dir.x + pos->x;
        end.y = length * dir.y + pos->y;
        UnkRay_l7399c4 ray(*pos, end);
        UnkRayHit_l7399c4 hit(*pos);
        // Cast untransformed.
        UnkFlags result = fn_7100d56604(reinterpret_cast<UnkWorld*>(Tester_l7399c4()),
                                        reinterpret_cast<const UnkQueryBox*>(&ray), shape,
                                        AsMtx34_l7399c4(Identity_l7399c4()), &hit, 2);
        if (!(result.bits & 1))
            continue;
        CVector2D at;
        fn_710007b430(&at, &hit.info.pos);
        f32 dx = pos->x - at.x;
        f32 dy = pos->y - at.y;
        distances[i] = sqrtf(dx * dx + dy * dy);
        if (nearest < 0 || distances[i] < distances[nearest])
            nearest = i;
    }
    if (nearest >= 0 && distances[nearest] < radius)
        radius = distances[nearest];
    return radius;
}
