// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py, unit 977): builds a list of up to `remaining`
// points along a direction: each step's centre, then points zigzagging sideways (55 units apart) while
// the collision tree and the owner's shape leave them free, turning back once at the first blocked one.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::global::CRntVector;
using base::math::CVector3D;

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

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
struct UnkMtx34;
UnkFlags fn_7100d56604(UnkWorld* self, const UnkQueryBox* box, UnkShape* shape, const UnkMtx34* xf, void* arg, u32 arg2);

extern void* g_unk_7101a5b258[];

// A 2D probe from the step's centre to a candidate point.
struct UnkPathProbe_l6533f0 {
    f32 fromX;
    f32 fromY;
    f32 toX;
    f32 toY;
    f32 deltaX;
    f32 deltaY;
};

// The hit filter the shape query fills in.
struct UnkPathHitFilter_l6533f0 {
    void** vtable;
    s32 field_0x8;
    u8 field_0xc[0x2c];
    bool hit;  // 0x38

    UnkPathHitFilter_l6533f0() : vtable(g_unk_7101a5b258 + 2), field_0x8(-1), field_0xc(), hit(false) {}
};

struct UnkPathOwner_l6533f0 {
    virtual void vf_0x0();
    UnkShape* Shape() {
        return reinterpret_cast<UnkShape* (*)(UnkPathOwner_l6533f0*)>((*reinterpret_cast<void***>(this))[0x1b8 / 8])(this);
    }
};

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* tester = g_unk_71726a47a0;
    if (!tester) {
        tester = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(tester);
        g_unk_71726a47a0 = tester;
    }
    return tester;
}

CRntVector<UnkVec3Result> fn_71006533f0(UnkPathOwner_l6533f0* self, u32 remaining, const CVector3D* start,
                                        const CVector3D* dir, u32 steps, f32 stepLength, u32* outCount) {
    CRntVector<UnkVec3Result> result(0xf);
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    UnkTree* tree = sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;

    UnkVec3Result center(start->x, start->y, start->z);
    f32 dirX = dir->x;
    f32 dirY = dir->y;
    f32 sideX = dirY * -55.0f;
    f32 sideY = dirX * 55.0f;
    f32 sideZ = 0.0f;
    u32 width = 1;
    for (u32 i = 0; remaining != 0 && i < steps; i++, width += 2) {
        *outCount = result.count;
        result.PushBackCopy(center);
        remaining--;

        UnkVec3Result pos = center;
        bool turned = false;
        u32 limit = width;
        s32 sign = 1;
        for (u32 j = 1; remaining != 0 && j < limit;) {
            f32 s = sign;
            f32 stepX = sideX * s;
            f32 stepY = sideY * s;
            f32 stepZ = s * sideZ;
            f32 offX = turned ? stepX : stepX * j;
            f32 offY = turned ? stepY : stepY * j;
            f32 offZ = turned ? stepZ : stepZ * j;
            pos.x = offX + pos.x;
            pos.y = offY + pos.y;
            pos.z = offZ + pos.z;

            UnkPathProbe_l6533f0 probe;
            probe.fromX = center.x;
            probe.fromY = center.y;
            probe.toX = pos.x - dirY * s * 50.0f;
            probe.toY = dirX * s * 50.0f + pos.y;
            probe.deltaX = probe.toX - center.x;
            probe.deltaY = probe.toY - center.y;
            bool blocked = fn_7100d5c518(tree, 3, reinterpret_cast<const UnkBox*>(&probe), nullptr, 2);
            if (!blocked) {
                UnkShape* shape = self->Shape();
                UnkPathHitFilter_l6533f0 filter;
                UnkShapeTester_A5_b68* tester = Tester();
                UnkMatrix44 identity = {};
                identity.m[15] = 1.0f;
                identity.m[10] = 1.0f;
                identity.m[5] = 1.0f;
                identity.m[0] = 1.0f;
                fn_7100d56604(reinterpret_cast<UnkWorld*>(tester), reinterpret_cast<const UnkQueryBox*>(&probe), shape,
                              reinterpret_cast<const UnkMtx34*>(&identity), &filter, 2);
                blocked = filter.hit;
            }
            if (blocked) {
                if (turned)
                    break;
                limit = (limit + j) / 2;
                sign = -sign;
                pos.x -= offX;
                pos.y -= offY;
                pos.z -= offZ;
                turned = true;
            } else {
                result.PushBackCopy(pos);
                j++;
                if (!turned)
                    sign = -sign;
                remaining--;
            }
        }

        center.x = center.x + dir->x * stepLength;
        center.y = center.y + dir->y * stepLength;
        center.z = center.z + dir->z * stepLength;
    }
    return result;
}
