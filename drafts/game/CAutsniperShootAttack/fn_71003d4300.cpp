// CAutsniperShootAttack: picks the point to aim at. Two navigation queries towards the target give
// candidate points; the closer usable one, pushed along the segment normal, becomes the aim point,
// unless it jumped away from the last one while a candidate sits near it. Names and layouts are placeholders.

#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPool_0x48;
struct UnkStruct_0x48;
UnkStruct_0x48* fn_710012431c(UnkPool_0x48* pool, u32 handle);
void fn_710007b444(void* dst, const CVector3D* src);
f32 fn_7100977b20(void*);
void fn_7100d62b04(void*);
void fn_7100d62bd8(void* p);

struct UnkPathQuery {
    u8 pad_0x0[0xa8];
    u8 field_0xa8[0x8];  // the pool of found polygons
    u32 field_0xb0;      // how many were found
    u8 pad_0xb4[0x120 - 0xb4];
};

UnkVec2Result fn_710035f248(void* nav, UnkStruct_0x48* polygon, const CVector3D* pos, UnkSegment2D* seg);
bool fn_71006f3494(void* registry, const CVector3D* pos, UnkPathQuery* first, UnkPathQuery* second);

struct UnkAimSelf {
    u8 pad_0x0[0x8];
    UnkActor* actor;
    u8 pad_0x10[0x6c];
    CVector3D last;  // the previous aim point
};

struct UnkAimPoint {
    f32 x, y, z;
    UnkAimPoint(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkAimPoint(const UnkAimPoint& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkManagerFields {
    u8 pad_0x0[0x928];
    u8 field_0x928[1];
};

static inline UnkManager_71726af468* GetManager() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

static inline CVector3D Pos(const UnkActor* actor) {
    CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}

template <class A, class B>
static inline bool IsNear2(const A& a, const B& b, f32 tol) {
    return a.x + tol >= b.x && a.x - tol <= b.x && a.y + tol >= b.y && a.y - tol <= b.y;
}

template <class A, class B>
static inline bool IsNear3(const A& a, const B& b, f32 tol) {
    return a.x + tol >= b.x && a.x - tol <= b.x && a.y + tol >= b.y && a.y - tol <= b.y &&
           a.z + tol >= b.z && a.z - tol <= b.z;
}

static inline f32 DistSq(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return dx * dx + dy * dy;
}

static inline CVector2D Pos2D(const UnkActor* actor) {
    CVector2D out;
    CVector3D p = Pos(actor);
    fn_710007b430(&out, &p);
    return out;
}

UnkAimPoint fn_71003d4300(UnkAimSelf* self, u8* snapped, u8* noPath) {
    UnkActor* target = fn_7100d7c990(self->actor);
    CVector3D own = Pos(self->actor);
    UnkAimPoint result(own.x, own.y, own.z);
    if (!target)
        return result;

    CVector2D best1;
    UnkPathQuery first;
    UnkPathQuery second;
    fn_7100d62b04(&first);
    fn_7100d62b04(&second);
    CVector3D targetPos = Pos(target);
    UnkManagerFields* manager = reinterpret_cast<UnkManagerFields*>(GetManager());
    if (fn_71006f3494(manager->field_0x928, &targetPos, &first, &second)) {
        best1.x = 0.0f;
        best1.y = 0.0f;
        CVector2D best2;
        best2.x = 0.0f;
        best2.y = 0.0f;
        UnkSegment2D seg;
        seg.field_0x10 = -1.0f;
        seg.start.x = 0.0f;
        seg.start.y = 0.0f;
        seg.end.x = 0.0f;
        seg.end.y = 0.0f;
        void* nav = static_cast<u8*>(self->actor->field_0x240) + 0x68;

        bool far1 = false;
        if (first.field_0xb0) {
            UnkStruct_0x48* polygon = fn_710012431c(reinterpret_cast<UnkPool_0x48*>(first.field_0xa8), 0);
            CVector3D p = Pos(target);
            UnkVec2Result r = fn_710035f248(nav, polygon, &p, &seg);
            best1.x = r.x;
            best1.y = r.y;
            if (IsNear2(best1, Pos2D(target), 1.0f)) {
                CVector3D o = Pos(self->actor);
                fn_710007b444(&best1, &o);
            } else {
                far1 = true;
            }
        }

        bool far2 = false;
        if (second.field_0xb0) {
            UnkStruct_0x48* polygon = fn_710012431c(reinterpret_cast<UnkPool_0x48*>(second.field_0xa8), 0);
            CVector3D p = Pos(target);
            UnkVec2Result r = fn_710035f248(nav, polygon, &p, &seg);
            best2.x = r.x;
            best2.y = r.y;
            if (IsNear2(best2, Pos2D(target), 1.0f)) {
                CVector3D o = Pos(self->actor);
                fn_710007b444(&best2, &o);
            } else {
                far2 = true;
            }
        }

        f32 x, y, z;
        if (far1 || far2) {
            CVector2D pick;
            if (far1 && far2)
                pick = DistSq(best1, Pos2D(target)) < DistSq(best2, Pos2D(target)) ? best1 : best2;
            else
                pick = far1 ? best1 : best2;
            result.x = pick.x;
            result.y = pick.y;
            result.z = 0.0f;
            x = pick.x;
            y = pick.y;
            z = 0.0f;
        } else {
            *noPath = 1;
            x = own.x;
            y = own.y;
            z = own.z;
        }

        UnkVec2Result dir = fn_7100125e78(&seg);
        f32 speed = fn_7100977b20(self->actor->field_0x218);
        result.x = x + dir.x * speed;
        result.y = dir.y * speed + y;
        result.z = z + speed * 0.0f;

        CVector3D cand1 = {best1.x, best1.y, 0.0f};
        CVector3D cand2 = {best2.x, best2.y, 0.0f};
        if (!IsNear3(self->last, result, 10.0f) &&
            (IsNear3(self->last, cand1, 5.0f) || IsNear3(self->last, cand2, 5.0f))) {
            *snapped = 1;
            result.x = self->last.x;
            result.y = self->last.y;
            result.z = self->last.z;
        } else {
            self->last.x = result.x;
            self->last.y = result.y;
            self->last.z = result.z;
        }
    }
    fn_7100d62bd8(&second);
    fn_7100d62bd8(&first);
    return result;
}
