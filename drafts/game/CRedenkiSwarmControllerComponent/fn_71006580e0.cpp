// CRedenkiSwarmControllerComponent's unit (977): probes the segment between two points against the swarm's
// collision shape (vf_0x1b8) and against the navigation mesh, and gives the nearer of the two contacts.
// Returns whether either was hit. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

struct UnkWorld;
struct UnkQueryBox;
struct UnkMtx34;
UnkFlags fn_7100d56604(UnkWorld* self, const UnkQueryBox* box, UnkShape* shape, const UnkMtx34* xf, void* arg, u32 arg2);

struct Result;
bool fn_7100ebfa7c(UnkNavMeshManager* self, Vec2* pos, Result* out, f32 a, u32 flags, f32 b);

extern void* g_unk_7101a23080[];

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// A segment: its ends and the offset between them.
struct UnkProbeSegment_l6580e0 {
    Vec2 start;
    Vec2 end;
    Vec2 delta;
    UnkProbeSegment_l6580e0(const Vec2& s, const Vec2& e) : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

// Where a probe touched: the point, the normal, and the distance along the probe (+0x24).
struct UnkProbeContact_l6580e0 {
    f32 x, y, z;
    f32 nx, ny, nz;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 distance;  // 0x24
    f32 field_0x28;
    UnkProbeContact_l6580e0()
        : x(0.0f), y(0.0f), z(0.0f), nx(0.0f), ny(0.0f), nz(0.0f), field_0x18(0.0f), field_0x1c(0.0f),
          field_0x20(0.0f), distance(0.0f), field_0x28(0.0f) {}
    UnkProbeContact_l6580e0& operator=(const UnkProbeContact_l6580e0& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        nx = o.nx;
        ny = o.ny;
        nz = o.nz;
        field_0x18 = o.field_0x18;
        field_0x1c = o.field_0x1c;
        field_0x20 = o.field_0x20;
        distance = o.distance;
        field_0x28 = o.field_0x28;
        return *this;
    }
};

// The collision probe's result (vtable g_unk_7101a23080).
struct UnkProbeHit_l6580e0 {
    void** vtable;
    s32 field_0x8;
    UnkProbeContact_l6580e0 contact;  // 0xc
    bool hit;                         // 0x38
    u8 pad_0x39[0x7];
    Vec2 origin;                      // 0x40
    u32 field_0x48;
    void* field_0x50;
    explicit UnkProbeHit_l6580e0(const Vec2& o)
        : vtable(g_unk_7101a23080 + 2), field_0x8(-1), hit(false), origin(o), field_0x48(0), field_0x50(nullptr) {}
};
static_assert(__builtin_offsetof(UnkProbeHit_l6580e0, origin) == 0x40, "layout");

// The navigation mesh probe's result: the cell and side, then the contact.
struct UnkProbeNavHit_l6580e0 {
    u16 cell;
    u8 side;
    UnkProbeContact_l6580e0 contact;  // 0x4
    UnkProbeNavHit_l6580e0() : cell(0xffff), side(0xff) {}
};

// The swarm's collision shape, with its transform at +0x8.
struct UnkProbeShape_l6580e0 {
    void* field_0x0;
    f32 transform[12];  // 0x8
};

struct UnkRedenkiSwarm_l6580e0;
struct UnkRedenkiSwarmVtable_l6580e0 {
    void* pad_0x0[0x1b8 / 8];
    UnkProbeShape_l6580e0* (*GetShape)(UnkRedenkiSwarm_l6580e0* self);  // 0x1b8
};
struct UnkRedenkiSwarm_l6580e0 {
    UnkRedenkiSwarmVtable_l6580e0* vtable;
};

bool fn_71006580e0(UnkRedenkiSwarm_l6580e0* self, const Vec2* from, const Vec2* to, UnkProbeContact_l6580e0* out,
                   f32 a, f32 b) {
    UnkProbeSegment_l6580e0 segment(*from, *to);
    UnkProbeShape_l6580e0* shape = self->vtable->GetShape(self);
    UnkProbeHit_l6580e0 hit(segment.start);
    fn_7100d56604(reinterpret_cast<UnkWorld*>(Tester()), reinterpret_cast<const UnkQueryBox*>(&segment),
                  reinterpret_cast<UnkShape*>(shape), reinterpret_cast<const UnkMtx34*>(shape->transform), &hit, 0);

    bool found = hit.hit;
    UnkProbeContact_l6580e0* best = found ? &hit.contact : nullptr;
    f32 best_distance = found ? hit.contact.distance : 3.4028235e38f;
    UnkProbeNavHit_l6580e0 nav;
    if (fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&nav), a, 0, b) &&
        nav.contact.distance < best_distance) {
        best = &nav.contact;
        found = true;
    }
    if (!found)
        return false;
    *out = *best;
    return true;
}
